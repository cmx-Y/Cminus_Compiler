#ifndef SYSYC_SELDAGNODES_HPP
#define SYSYC_SELDAGNODES_HPP

#include "Module.h"
#include "BasicBlock.h"
#include "Instruction.h"
#include "Type.h"

//idea：由于Instruction类具有多个派生类，所以可以利用函数的多态
//      对不同的指令进行不同的BuildSDNode
//idea: AsmPrinter好像得用访问者模式才能实现，现在我得探索一下怎么设计派生类
//      /include/llvm/CodeGen/SelectionDAGNodes.h     line 1253
//idea: 由于Instruction_list是线性的，所以可以直接用一堆if-else来建立DAG
//      由于DAG是非线性的，所以需要visitor pattern来实现assemble language print
//idea: 之后在DAG上可能需要写的pass：合法化，指令选择，寄存器分配

//首先保证正确性，而且光是保持正确性，工作量就已经不小了！倘若没有正确性，一切都是空中楼阁！！

//TODO: 如何得到符合规范且能正确运行的riscv代码
//      打印出双加法的riscv代码
//      写出AsmPrinter 的 visitor pattern框架   ✔
//      边建DAG，边新建派生类
//      通过第一个测试样例！
//      回顾visitor pattern，明确SDBuild与ASMPrinter过程需采用的技术路线    ✔
//      弄清risc-v汇编规范
//      打印出对应的risc-v汇编代码

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

// 每个SDUse表示对一个SDNode的引用.
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
  SDValue _val; // 该SDUse实际引用的SDValue
  SDNode *_user = nullptr; // 指向引用该SDNode的节点(引用者)
  //SDUse *Prev = nullptr; // 双向链表记录所有引用该SDNode的SDUse
  //SDUse *Next = nullptr; // 双向链表记录所有引用该SDNode的SDUse
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
    std::list<SDUse*> _operand_list;  //表示黑色实线，数据依赖
    int _num_operands = 0;
    std::list<Value*> _value_list;    //TODO: 为什么有多个输出？？
    int _num_values = 0;
    std::list<SDUse*> _use_list; // 引用该节点的链表

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
    };  //这里的enum类型，为ASMPrinter而服务，即具体的指令选择

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