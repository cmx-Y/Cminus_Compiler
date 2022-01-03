#include "Module.h"
#include "BasicBlock.h"
#include "Instruction.h"
#include "Type.h"
#include "AsmPrinter.hpp"
#include "Constant.h"

SDNode::SDNode(Type *node_type, Instruction::OpID node_op) : _node_type(node_type), _node_op(node_op) { 

}

SDNode::SDNode(Type *node_type) : _node_type(node_type) { 

}

SDNode::~SDNode() = default;

void SDNode::add_value(Value *val) {
    _value_list.push_back(val);
    _num_values++;
}

void SDNode::add_operand(SDNode *node, int res_no, SDNode *user){
    auto sduse_ptr = new SDUse(node, res_no, user);
    _operand_list.push_back(sduse_ptr);
    _num_operands++;
}

void SDNode::add_use(SDUse* use){ 
    _use_list.push_back(use);
}

SDUse* SDNode::get_operand(int no) {
    int i = 0;
    for(auto it = _operand_list.begin(); it != _operand_list.end(); it++, i++){
        if(i == no) return *it;
    }
    return nullptr;
}

Value* SDNode::get_val(int no) {
    int i = 0;
    for(auto it = _value_list.begin(); it != _value_list.end(); it++, i++){
        if(i == no) return *it;
    }
    return nullptr;
}

SDUse* SDNode::get_use(int no) {
    int i = 0;
    for(auto it = _use_list.begin(); it != _use_list.end(); it++, i++){
        if(i == no) return *it;
    }
    return nullptr;
}

void SelDAGBuilder::run(){
    std::map<std::string, SDNode *> varname_2_SDNode;
    auto func_list = m_->get_functions();
    for(auto func : func_list)
    {
        int var_no = func->get_num_of_args() + 3;
        int func_frame_size = 32;

        if(func->get_name() != "input" && func->get_name() != "outputFloat" && func->get_name() != "neg_idx_except"){
            auto func_dag_root = new RootSDNode();
            auto prolog = new PrologSDNode(func->get_name());
            prolog->set_arg_num(func->get_num_of_args());
            func_dag_root->set_chain_depen(prolog);
            _dag_root_list.push_back(func_dag_root);
        }


        for(auto bb : func->get_basic_blocks())
        {
            auto bb_dag_root = new RootSDNode();
            for(auto instr : bb->get_instructions())
            {
                if(instr->isBinary()){
                    BinarySDNode* l_SDNode = nullptr;
                    BinarySDNode* r_SDNode = nullptr;
                    if(instr->get_operand(0)->get_name().substr(0,3) == "arg"){
                        l_SDNode = new BinarySDNode();
                        int arg_no = std::stoi(instr->get_operand(0)->get_name().substr(3,1));
                        l_SDNode->set_stack_addr(12 + 4*arg_no);
                        l_SDNode->set_node_name(instr->get_operand(0)->get_name());
                        varname_2_SDNode.insert({l_SDNode->get_node_name(), l_SDNode});
                    }
                    else if(instr->get_operand(0)->get_name() == ""
                    || varname_2_SDNode.find(instr->get_operand(0)->get_name()) == varname_2_SDNode.end())
                    {
                        l_SDNode = new BinarySDNode(instr->get_operand(0)->get_type());
                        l_SDNode->add_value(instr->get_operand(0));
                        if(instr->get_operand(0)->get_name() == ""){
                            auto operand_val = (ConstantInt*)(instr->get_operand(0));
                            l_SDNode->set_is_const();
                            l_SDNode->set_const_val(operand_val->get_value());
                        }
                        else{
                            l_SDNode->set_stack_addr(var_no*4);
                            var_no++;
                        }
                        l_SDNode->set_node_name(instr->get_operand(0)->get_name());
                        varname_2_SDNode.insert({l_SDNode->get_node_name(), l_SDNode});
                    }
                    else{
                        l_SDNode = (BinarySDNode*)varname_2_SDNode.find(instr->get_operand(0)->get_name())->second;
                    }

                    if(instr->get_operand(1)->get_name().substr(0,3) == "arg"){
                        r_SDNode = new BinarySDNode();
                        int arg_no = std::stoi(instr->get_operand(1)->get_name().substr(3,1));
                        r_SDNode->set_stack_addr(12 + 4*arg_no);
                        r_SDNode->set_node_name(instr->get_operand(1)->get_name());
                        varname_2_SDNode.insert({r_SDNode->get_node_name(), r_SDNode});
                    }
                    if(instr->get_operand(1)->get_name() == ""
                    || varname_2_SDNode.find(instr->get_operand(1)->get_name()) == varname_2_SDNode.end())
                    {
                        r_SDNode = new BinarySDNode(instr->get_operand(1)->get_type());
                        r_SDNode->add_value(instr->get_operand(1));
                        if(instr->get_operand(1)->get_name() == ""){
                            auto operand_val = (ConstantInt*)(instr->get_operand(1));
                            r_SDNode->set_is_const();
                            r_SDNode->set_const_val(operand_val->get_value());
                        }
                        else{
                            r_SDNode->set_stack_addr(var_no*4);
                            var_no++;
                        }
                        r_SDNode->set_node_name(instr->get_operand(1)->get_name());
                        varname_2_SDNode.insert({r_SDNode->get_node_name(), r_SDNode});
                    }
                    else{
                        r_SDNode = (BinarySDNode*)varname_2_SDNode.find(instr->get_operand(1)->get_name())->second;
                    }

                    auto op_SDNode = new BinarySDNode(instr->get_operand(0)->get_type(), instr->get_instr_type());
                    op_SDNode->add_value(instr);        //what's op_SDNode's value
                    op_SDNode->set_node_name(instr->get_name());
                    op_SDNode->set_stack_addr(var_no*4);
                    var_no++;
                    varname_2_SDNode.insert({op_SDNode->get_node_name(), op_SDNode});
                    op_SDNode->add_operand(l_SDNode, 0, op_SDNode);
                    l_SDNode->add_use(op_SDNode->get_operand(0));
                    op_SDNode->add_operand(r_SDNode, 1, op_SDNode);
                    r_SDNode->add_use(op_SDNode->get_operand(1));
                    bb_dag_root->set_chain_depen(op_SDNode);
                }
                else if(instr->isTerminator()){
                    if(instr->is_ret()){
                        auto ret_instr = (ReturnInst *)instr;
                        auto ret_SDNode = new ReturnSDNode();
                        if(func->get_name() == "main")
                            ret_SDNode->set_should_exit();
                        ret_SDNode->set_frame_size(func_frame_size);
                        ret_SDNode->set_chain_depen(bb_dag_root->get_chain_depen());
                        bb_dag_root->set_chain_depen(ret_SDNode);
                        _dag_root_list.push_back(bb_dag_root);
                        if(ret_instr->is_void_ret()){

                        }
                        else{
                            ret_SDNode->set_has_ret_val();
                            auto ret_val_SDNode = (BinarySDNode *)varname_2_SDNode.find(ret_instr->get_operand(0)->get_name())->second;
                            int ret_val_addr = ret_val_SDNode->get_stack_addr();
                            ret_SDNode->set_ret_val_addr(ret_val_addr);
                        }
                    }
                    else if(instr->is_br()){

                    }
                    //create dag root, and push it into stack
                }
                else if(instr->is_call()){
                    std::cout << "instr_name:" << instr->get_name() << std::endl;
                    std::cout << "arg_num:" << instr->get_num_operand() << std::endl;
                    std::cout << "arg_0:" << instr->get_operand(0)->get_name() << std::endl;
                    std::cout << "arg_1:" << instr->get_operand(1)->get_name() << std::endl;
                    auto call_SDNode = new CallSDNode();
                    for(int i = 0; i < instr->get_num_operand(); i++){
                        if(i == 0){
                            call_SDNode->set_func_name(instr->get_operand(0)->get_name());
                        }
                        else{
                            auto arg_name = instr->get_operand(i)->get_name();
                            auto cor_SDNode = (BinarySDNode *)(varname_2_SDNode.find(arg_name)->second);
                            call_SDNode->add_arg_addr(cor_SDNode->get_stack_addr());
                        }
                    }
                    call_SDNode->set_arg_num(instr->get_num_operand() - 1);
                    
                    call_SDNode->set_chain_depen(bb_dag_root->get_chain_depen());
                    bb_dag_root->set_chain_depen(call_SDNode);
                    if(instr->get_name() != ""){
                        auto ret_user_SDNode = new BinarySDNode();
                        ret_user_SDNode->set_is_call_assign();
                        ret_user_SDNode->add_operand(call_SDNode, 0,ret_user_SDNode);
                        call_SDNode->add_use(ret_user_SDNode->get_operand(0));
                        ret_user_SDNode->set_node_name(instr->get_name());
                        ret_user_SDNode->set_stack_addr(var_no*4);
                        var_no++;
                        call_SDNode->set_ret_user_addr(ret_user_SDNode->get_stack_addr());
                        varname_2_SDNode.insert({ret_user_SDNode->get_node_name(), ret_user_SDNode});
                        bb_dag_root->set_chain_depen(ret_user_SDNode);
                    }
                }
                //etc. else if...
            }
        }
    }
}

SDNode* SelDAGBuilder::get_root(int no){
    int i = 0;
    for(auto it = _dag_root_list.begin(); it != _dag_root_list.end(); it++, i++){
        if(i == no) return *it;
    }
    return nullptr;
}

int CallSDNode::get_argi_addr(int no){
    int i = 0;
    for(auto it = _arg_addr_list.begin(); it != _arg_addr_list.end(); it++, i++){
        if(i == no) return *it;
    }
    return -1;
}


std::string BinarySDNode::accept(SDNodeVisitor &visitor) { return visitor.visit(*this);}
std::string PrologSDNode::accept(SDNodeVisitor &visitor) { return visitor.visit(*this);}
std::string RootSDNode::accept(SDNodeVisitor &visitor) { return visitor.visit(*this);}
std::string ReturnSDNode::accept(SDNodeVisitor &visitor) { return visitor.visit(*this);}
std::string CallSDNode::accept(SDNodeVisitor &visitor) { return visitor.visit(*this);}