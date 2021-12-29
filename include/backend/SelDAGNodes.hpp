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

//���ȱ�֤��ȷ�ԣ����ҹ��Ǳ�����ȷ�ԣ����������Ѿ���С�ˣ�����û����ȷ�ԣ�һ�ж��ǿ���¥�󣡣�

//TODO: ��εõ����Ϲ淶������ȷ���е�riscv����
//      ��ӡ��˫�ӷ���riscv����
//      д��AsmPrinter �� visitor pattern���   �7�8
//      �߽�DAG�����½�������
//      ͨ����һ������������
//      �ع�visitor pattern����ȷSDBuild��ASMPrinter��������õļ���·��    �7�8
//      Ū��risc-v���淶
//      ��ӡ����Ӧ��risc-v������

class SDUse;
class SDValue;
class SDNode;
class BinarySDNode;
class RISCV_td;


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
    virtual void visit(BinarySDNode &node) = 0;
};

class SDNode{
public:
    SDNode(Type *node_type, Instruction::OpID node_op);
    SDNode(Type *node_type);
    virtual ~SDNode();

    virtual void accept(SDNodeVisitor &visitor) { }

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

    void set_node_name(std::string name) { _node_name = name;}
    
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

    //TODO: chain dependence, glue dependence
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

    virtual void accept(SDNodeVisitor &visitor) override final;

private:
    RISCV_td::REGISTER _reg;
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

private:
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