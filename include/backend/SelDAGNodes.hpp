#ifndef SYSYC_SELDAGNODES_HPP
#define SYSYC_SELDAGNODES_HPP

#include "Module.h"
#include "BasicBlock.h"
#include "Instruction.h"
#include "Type.h"

//idea������Instruction����ж�������࣬���Կ������ú����Ķ�̬
//      �Բ�ͬ��ָ����в�ͬ��BuildSDNode
//idea: AsmPrinter������÷�����ģʽ����ʵ�֣������ҵ�̽��һ����ô���������
//      /include/llvm/CodeGen/SelectionDAGNodes.h     line 1253
//idea: ����Instruction_list�����Եģ����Կ���ֱ����һ��if-else������DAG
//      ����DAG�Ƿ����Եģ�������Ҫvisitor pattern��ʵ��assemble language print
//idea: ֮����DAG�Ͽ�����Ҫд��pass���Ϸ�����ָ��ѡ�񣬼Ĵ�������

//main trouble: return void ��DAG�е����߹�ϵ��ô�㣿����

//TODO: ͨ��llc�۲�void_return��ηŽ�DAG
//      SelectionDAGBuilder�м���all_dag_roots
//      SelectionDAGBuilder::run��дreturn_void
//      ��ӡ����һ������������RISC-v����

class RISCV_td;
class SDUse;
class SDValue;
class SDNode;
class BinarySDNode;
class RootSDNode;
class PrologSDNode;
class ReturnSDNode;


class RISCV_td{
public:
    enum REGISTER
    {
        zero,
        ra,     //return address
        sp,     //stack pointer
        gp,     //global pointer
        tp,     //thread pointer
        t0,     //temporary
        t1,
        t2,
        s0,     //frame pointer
        s1,
        a0,     //return value
        a1,     //return value
        a2,     //function argument
        a3,
        a4,
        a5,
        a6,
        a7,
        s2,     //saved register
        s3,
        s4,
        s5,
        s6,
        s7,
        s8,
        s9,
        s10,
        t3,
        t4,
        t5,
        t6,
        pc,
    };
};

class SDValue{
public:
    void set_node(SDNode *node) { _node = node;}
    void set_res_no(int res_no) {_res_no = res_no;}
    SDNode* get_node() { return _node;}
    int get_res_no() { return _res_no;}

private:    
    SDNode *_node = nullptr;
    int _res_no = 0;
};

// ÿ��SDUse��ʾ��һ��SDNode������.
class SDUse {
public:
    SDUse(SDNode *node, int res_no, SDNode *user){
        _val.set_node(node);
        _val.set_res_no(res_no);
        _user = user;
    }
    void set_user(SDNode *user) {_user = user;}
    SDValue* get_val() { return &_val;}
    SDNode* get_user() { return _user;}

private:
  SDValue _val; // ��SDUseʵ�����õ�SDValue
  SDNode *_user = nullptr; // ָ�����ø�SDNode�Ľڵ�(������)
  //SDUse *Prev = nullptr; // ˫�������¼�������ø�SDNode��SDUse
  //SDUse *Next = nullptr; // ˫�������¼�������ø�SDNode��SDUse
};

class SDNodeVisitor{
public:
    virtual std::string visit(BinarySDNode &node) = 0;
    virtual std::string visit(PrologSDNode &node) = 0;
    virtual std::string visit(RootSDNode &node) = 0;
    virtual std::string visit(ReturnSDNode &node) = 0;
};

class SDNode{
public:
    SDNode() = default;
    SDNode(Type *node_type, Instruction::OpID node_op);
    SDNode(Type *node_type);
    virtual ~SDNode();

    virtual std::string accept(SDNodeVisitor &visitor) { }

    void add_value(Value *val);
    void add_operand(SDNode *node, int res_no, SDNode *user);
    void add_use(SDUse* use);

    SDUse* get_operand(int no);
    Value* get_val(int no);
    SDUse* get_use(int no);
    Type* get_node_type() { return _node_type;}
    Instruction::OpID get_node_op() { return _node_op;}
    int get_node_id() { return _node_id;}
    int get_num_operands() { return _num_operands;}
    int get_num_values() { return _num_values;}
    int get_num_users() { return _use_list.size();}
    std::string get_node_name() { return _node_name;}
    SDNode *get_chain_depen() { return chain;}

    void set_node_name(std::string name) { _node_name = name;}
    void set_chain_depen(SDNode *be_pointed) { chain = be_pointed;}
    
private:
    Type *_node_type;
    Instruction::OpID _node_op = Instruction::OpID::nop;
    int _node_id;
    std::string _node_name;
    std::list<SDUse*> _operand_list;  //��ʾ��ɫʵ�ߣ���������
    int _num_operands = 0;
    std::list<Value*> _value_list;    //TODO: Ϊʲô�ж���������
    int _num_values = 0;
    std::list<SDUse*> _use_list; // ���øýڵ������
    SDNode* chain;  //chain dependence

    //TODO: chain dependence, glue dependence
};

class RootSDNode : public SDNode{
public:
    virtual std::string accept(SDNodeVisitor &visitor) override final;

private:

};

class BinarySDNode : public SDNode{
public:
    enum MCBinaryType
    {
        add,
        addi,
        sub,
        subi,
    };  //�����enum���ͣ�ΪASMPrinter�����񣬼������ָ��ѡ��

    BinarySDNode(Type *node_type) : SDNode(node_type) { }
    BinarySDNode(Type *node_type, Instruction::OpID node_op) : SDNode(node_type, node_op) { }

    virtual std::string accept(SDNodeVisitor &visitor) override final;

    int get_stack_addr() { return _stack_addr;}

private:
    RISCV_td::REGISTER _reg;
    int _stack_addr;
};

class PrologSDNode : public SDNode{
public:
    PrologSDNode() = default;
    PrologSDNode(std::string func_name) : _func_name(func_name) { }
    
    virtual std::string accept(SDNodeVisitor &visitor) override final;

    std::string get_func_name() { return _func_name;}
    int get_frame_size() { return _frame_size;}
private:
    std::string _func_name;
    int _frame_size = 32;
    int _arg_num;
};

class ReturnSDNode : public SDNode{
public:
    virtual std::string accept(SDNodeVisitor &visitor) override final;

private:

};

class RegSDNode : public SDNode{
public:

private:

};

class CmpSDNode : public SDNode{

};

class MemSDNode : public SDNode{

private:
    //MemoryType
    //enum load, store
};

class SelDAGBuilder{
public:
    SelDAGBuilder(Module *m) : m_(m) { }
    void run();
    SDNode *get_dag_root() { return _dag_root;}
    int get_root_num() { return _dag_root_list.size();}
    SDNode* get_root(int no);
    void add_dag_root(RootSDNode *root) { _dag_root_list.push_back(root);}
    
    

private:
    //vector,dag_root_list
    std::list<SDNode *> _dag_root_list;
    SDNode *_dag_root;
    Module *m_;
};


class SelectionDAG{
public:
    SDNode *get_DAG_root() { return Root;}

private:
    //TODO: what's TargetLowering and FunctionLowering
    //const TargetLowering *TLI = nullptr;
    //FunctionLoweringInfo * FLI = nullptr;
    SDNode EntryNode;   //TODO: lable type?? what's entry node???

    //SDValue Root;   //TODO: why type SDValue??
    SDNode *Root;

    std::list<SDNode> AllNodes;

};




#endif