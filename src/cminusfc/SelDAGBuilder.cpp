#include "Module.h"
#include "BasicBlock.h"
#include "Instruction.h"
#include "Type.h"

//idea：由于Instruction类具有多个派生类，所以可以利用函数的多态
//      对不同的指令进行不同的BuildSDNode
//idea: AsmPrinter好像得用访问者模式才能实现，现在我得探索一下怎么设计派生类
//      /include/llvm/CodeGen/SelectionDAGNodes.h     line 1253

//TODO: 检查数据依赖关系并打印清楚              ✔
//      分离头文件，写出visitor pattern框架
//      弄清risc-v汇编规范
//      打印出对应的risc-v汇编代码
class SDUse;
class SDNode;
class SDValue;

class SDValue{
    friend class SDUse;
public:
    //SDValue();
    //SDValue(SDNode *node, int res_no) : _node(node), _res_no(res_no) { }
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

class SDNode{
public:
    SDNode(Type *node_type, Instruction::OpID node_op) : _node_type(node_type), _node_op(node_op) { }
    SDNode(Type *node_type) : _node_type(node_type) { }
    void add_value(Value *val) {
        _value_list.push_back(val);
        _num_values++;
    }
    void add_operand(SDNode *node, int res_no, SDNode *user){
        auto sduse_ptr = new SDUse(node, res_no, user);
        _operand_list.push_back(sduse_ptr);
        _num_operands++;
    }
    void add_use(SDUse* use){ _use_list.push_back(use);}
    SDUse* get_operand(int no) {
        int i = 0;
        for(auto it = _operand_list.begin(); it != _operand_list.end(); it++, i++){
            if(i == no) return *it;
        }
        return nullptr;
    }
    Value* get_val(int no) {
        int i = 0;
        for(auto it = _value_list.begin(); it != _value_list.end(); it++, i++){
            if(i == no) return *it;
        }
        return nullptr;
    }
    SDUse* get_use(int no) {
        int i = 0;
        for(auto it = _use_list.begin(); it != _use_list.end(); it++, i++){
            if(i == no) return *it;
        }
        return nullptr;
    }
    Type* get_node_type() { return _node_type;}
    Instruction::OpID get_node_op() { return _node_op;}
    int get_node_id() { return _node_id;}
    int get_num_operands() { return _num_operands;}
    int get_num_values() { return _num_values;}
    int get_num_users() { return _use_list.size();}
    void set_node_name(std::string name) {
        _node_name = name;        
    }
    std::string get_node_name() { return _node_name;}


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


class SelDAGBuilder{
public:
    SelDAGBuilder(Module *m) : m_(m) { }
    //~SelDAGBuilder(){}
    void run(){
        std::map<std::string, SDNode *> varname_2_SDNode;
        auto func_list = m_->get_functions();
        for(auto func : func_list)
        {
            for(auto bb : func->get_basic_blocks())
            {
                for(auto instr : bb->get_instructions())
                {
                    if(instr->isBinary()){
                        std::cout << "what????" << std::endl;
                        SDNode* l_SDNode = nullptr;
                        SDNode* r_SDNode = nullptr;
                        //TODO: 要检查是否需要新建SDNode 
                        if(instr->get_operand(0)->get_name() == ""
                        || varname_2_SDNode.find(instr->get_operand(0)->get_name()) == varname_2_SDNode.end())
                        {
                            l_SDNode = new SDNode(instr->get_operand(0)->get_type());
                            l_SDNode->add_value(instr->get_operand(0));
                            l_SDNode->set_node_name(instr->get_operand(0)->get_name());
                            varname_2_SDNode.insert({l_SDNode->get_node_name(), l_SDNode});
                        }
                        else{
                            l_SDNode = varname_2_SDNode.find(instr->get_operand(0)->get_name())->second;
                        }

                        if(instr->get_operand(1)->get_name() == ""
                        || varname_2_SDNode.find(instr->get_operand(1)->get_name()) == varname_2_SDNode.end())
                        {
                            r_SDNode = new SDNode(instr->get_operand(1)->get_type());
                            r_SDNode->add_value(instr->get_operand(1));
                            r_SDNode->set_node_name(instr->get_operand(1)->get_name());
                            varname_2_SDNode.insert({r_SDNode->get_node_name(), r_SDNode});
                        }
                        else{
                            r_SDNode = varname_2_SDNode.find(instr->get_operand(1)->get_name())->second;
                        }

                        auto op_SDNode = new SDNode(instr->get_operand(0)->get_type(), instr->get_instr_type());
                        op_SDNode->add_value(instr);        //what's op_SDNode's value
                        op_SDNode->set_node_name(instr->get_name());
                        varname_2_SDNode.insert({op_SDNode->get_node_name(), op_SDNode});
                        op_SDNode->add_operand(l_SDNode, 0, op_SDNode);
                        l_SDNode->add_use(op_SDNode->get_operand(0));
                        op_SDNode->add_operand(r_SDNode, 1, op_SDNode);
                        r_SDNode->add_use(op_SDNode->get_operand(1));
                        
                        std::cout << "===l_SDNode===" << std::endl;
                        std::cout << "l_op_id: " << l_SDNode->get_node_op() << std::endl;
                        std::cout << "l_type_id: " << l_SDNode->get_node_type()->get_type_id() << std::endl;
                        std::cout << "l_num_operands: " << l_SDNode->get_num_operands() << std::endl;
                        if(l_SDNode->get_num_operands() != 0){
                            for(int i = 0; i < l_SDNode->get_num_operands(); i++){
                                auto operandi = l_SDNode->get_operand(i);
                                auto operand_node = operandi->get_val()->get_node();
                                std::cout << "operand" << i << " " << operand_node->get_node_name() << std::endl;
                            }
                        }
                        std::cout << "l_num_users: " << l_SDNode->get_num_users() << std::endl;
                        if(l_SDNode->get_num_users() != 0){
                            for(int i = 0; i < l_SDNode->get_num_users(); i++){
                                auto usei = l_SDNode->get_use(i);
                                std::cout << "user" << i << " " << usei->get_user()->get_node_name() << " ";
                                std::cout << "no " << usei->get_val()->get_res_no() << std::endl;
                            }
                        }
                        std::cout << "l_num_vals: " << l_SDNode->get_num_values() << std::endl;
                        std::cout << "l_node_name:" << l_SDNode->get_node_name() << std::endl;
                        
                        
                        std::cout << "===r_SDNode===" << std::endl;
                        std::cout << "r_op_id: " << r_SDNode->get_node_op() << std::endl;
                        std::cout << "r_type_id: " << r_SDNode->get_node_type()->get_type_id() << std::endl;
                        std::cout << "r_num_operands: " << r_SDNode->get_num_operands() << std::endl;
                        std::cout << "r_num_users: " << r_SDNode->get_num_users() << std::endl;
                        if(r_SDNode->get_num_users() != 0){
                            for(int i = 0; i < r_SDNode->get_num_users(); i++){
                                auto usei = r_SDNode->get_use(i);
                                std::cout << "user" << i << " " << usei->get_user()->get_node_name() << " ";
                                std::cout << "no " << usei->get_val()->get_res_no() << std::endl;
                            }
                        }
                        std::cout << "r_num_vals: " << r_SDNode->get_num_values() << std::endl;
                        std::cout << "r_node_name:" << r_SDNode->get_node_name() << std::endl;

                        std::cout << "===op_SDNode===" << std::endl;
                        std::cout << "op_op_id: " << op_SDNode->get_node_op() << std::endl;
                        std::cout << "op_type_id: " << op_SDNode->get_node_type()->get_type_id() << std::endl;
                        std::cout << "op_num_opeopands: " << op_SDNode->get_num_operands() << std::endl;
                        if(op_SDNode->get_num_operands() != 0){
                            for(int i = 0; i < op_SDNode->get_num_operands(); i++){
                                auto operandi = op_SDNode->get_operand(i);
                                auto operand_node = operandi->get_val()->get_node();
                                std::cout << "operand" << i << " " << operand_node->get_node_name() << std::endl;
                            }
                        }
                        std::cout << "op_num_users: " << op_SDNode->get_num_users() << std::endl;
                        if(op_SDNode->get_num_users() != 0){
                            for(int i = 0; i < op_SDNode->get_num_users(); i++){
                                auto usei = op_SDNode->get_use(i);
                                std::cout << "user" << i << " " << usei->get_user()->get_node_name() << " ";
                                std::cout << "no " << usei->get_val()->get_res_no() << std::endl;
                            }
                        }
                        std::cout << "op_num_vals: " << op_SDNode->get_num_values() << std::endl;
                        std::cout << "op_node_name:" << op_SDNode->get_node_name() << std::endl;
                    }
                }
            }
        }
    }

private:
    Module *m_;
};

class SelectionDAG{
public:

private:
    //TODO: what's TargetLowering and FunctionLowering
    //const TargetLowering *TLI = nullptr;
    //FunctionLoweringInfo * FLI = nullptr;
    SDNode EntryNode;   //TODO: lable type??

    SDValue Root;   //TODO: why type SDValue??

    std::list<SDNode> AllNodes;

};
