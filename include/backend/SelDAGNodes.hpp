#ifndef SYSYC_SELDAGNODES_HPP
#define SYSYC_SELDAGNODES_HPP

#include "Module.h"
#include "BasicBlock.h"
#include "Instruction.h"
#include "Type.h"

//idea: 在SelectionDAG上需要写的pass：
//      基础pass：合法化&&指令选择
//      扩展pass：寄存器分配

//idea: 在每个func上写一个pass，计算每个函数所需的最大栈空间(frame_size)
//      但以目前的情况来看，可以先分配足够大的栈空间

//hit:  about output
//      need system call, which system call is specified by a7
//      when call PrintInt, argument in a7 is 1
//      and then I should use ecall to wake system call

//TODO: 判断某个操作数是否是常数，如果是常数，不用分配栈空间，直接mv进寄存器    ✔
//      完成add指令                                                         ✔
//      打印函数时要在prologue中加上函数参数分配                              ✔
//      要完成output函数                                                    ✔
//      (完成output函数，不仅能解决怎么输出的问题，还能解决函数调用的问题)     ✔
//      打印出第一个测试样例的RISC-v代码

class RISCV_td;
class SDUse;
class SDValue;
class SDNode;
class BinarySDNode;
class RootSDNode;
class PrologSDNode;
class ReturnSDNode;
class CallSDNode;


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
    virtual std::string visit(BinarySDNode &node) = 0;
    virtual std::string visit(PrologSDNode &node) = 0;
    virtual std::string visit(RootSDNode &node) = 0;
    virtual std::string visit(ReturnSDNode &node) = 0;
    virtual std::string visit(CallSDNode &node) = 0;
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
    std::list<SDUse*> _operand_list;  //表示黑色实线，数据依赖
    int _num_operands = 0;
    std::list<Value*> _value_list;    //TODO: 为什么有多个输出？？
    int _num_values = 0;
    std::list<SDUse*> _use_list; // 引用该节点的链表
    SDNode* chain;  //chain dependency

    //TODO: chain dependency, glue dependency
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
    };  //这里的enum类型，为ASMPrinter而服务，即具体的指令选择
        //不要选择了，我直接暂时全用add！

    BinarySDNode() = default;
    BinarySDNode(Type *node_type) : SDNode(node_type) { }
    BinarySDNode(Type *node_type, Instruction::OpID node_op) : SDNode(node_type, node_op) { }

    virtual std::string accept(SDNodeVisitor &visitor) override final;

    bool is_const() { return _is_const;}
    bool is_call_assign() { return _is_call_assign;}

    int get_stack_addr() { return _stack_addr;}
    int get_const_val() { return _const_val;}
    
    void set_is_call_assign() { _is_call_assign = true;}
    void set_is_const() { _is_const = true;}
    void set_const_val(int const_val) { _const_val = const_val;}
    void set_stack_addr(int stack_addr) { _stack_addr = stack_addr;}

private:
    bool _is_call_assign = false;
    bool _is_const = false;
    int _const_val = -1;
    RISCV_td::REGISTER _reg;
    int _stack_addr;
};

class PrologSDNode : public SDNode{
public:
    PrologSDNode() = default;
    PrologSDNode(std::string func_name) : _func_name(func_name) { }
    
    virtual std::string accept(SDNodeVisitor &visitor) override final;

    void set_arg_num(int arg_num) { _arg_num = arg_num;}
    int  get_arg_num() { return _arg_num;}
    std::string get_func_name() { return _func_name;}
    int get_frame_size() { return _frame_size;}
private:
    std::string _func_name;
    int _frame_size = 32;
    int _arg_num = 0;
};

class ReturnSDNode : public SDNode{
public:
    virtual std::string accept(SDNodeVisitor &visitor) override final;
    void set_frame_size(int frame_size) { _frame_size = frame_size;}
    int get_frame_size() { return _frame_size;}
    bool get_should_exit() { return _should_exit;}
    void set_should_exit() { _should_exit = true;}
    bool get_has_ret_val() { return _has_ret_val;}
    void set_has_ret_val() { _has_ret_val = true;}
    int get_ret_val_addr() { return _ret_val_addr;}
    void set_ret_val_addr(int addr) { _ret_val_addr = addr;}

private:
    int _frame_size;
    bool _should_exit = false;
    bool _has_ret_val = false;
    int _ret_val_addr;
};

class CallSDNode : public SDNode{
public:

    virtual std::string accept(SDNodeVisitor &visitor) override final;

    void set_arg_num(int arg_num) { _arg_num = arg_num;}
    void set_func_name(std::string func_name) { _func_name = func_name;}
    void add_arg_addr(int addr) { _arg_addr_list.push_back(addr);}
    void set_ret_user_addr(int addr) { _ret_user_addr = addr;}

    std::string get_func_name() { return _func_name;}
    int get_arg_num() { return _arg_num;}
    int get_argi_addr(int no);
    int get_ret_user_addr() { return _ret_user_addr;}


private:
    //要注意一下有关返回值的问题
    int _ret_user_addr;
    std::vector<int> _arg_addr_list;
    std::string _func_name;
    int _arg_num;

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
    int get_root_num() { return _dag_root_list.size();}
    SDNode* get_root(int no);
    void add_dag_root(RootSDNode *root) { _dag_root_list.push_back(root);}
    
    

private:
    //vector,dag_root_list
    std::list<SDNode *> _dag_root_list;
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