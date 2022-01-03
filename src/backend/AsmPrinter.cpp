#include "AsmPrinter.hpp"

int reg_cnt = 1;

std::string AsmPrinter::visit(BinarySDNode &node){
    std::string ASM;
    if(node.get_num_operands() == 0){
        if(node.is_const()){
            ASM += "\tli\t";
            ASM += "a";
            ASM += std::to_string(reg_cnt);
            ASM += ", ";
            ASM += std::to_string(node.get_const_val());
            ASM += "\n";
            reg_cnt++;
            if(reg_cnt == 3) reg_cnt = 1;
        }
        else{
            ASM += "\tlw\t";
            ASM += "a";
            ASM += std::to_string(reg_cnt);
            ASM += ", ";
            ASM += "-";
            ASM += std::to_string(node.get_stack_addr());
            ASM += "(s0)";
            ASM += "\n";
            reg_cnt++;
            if(reg_cnt == 3) reg_cnt = 1;
        }
        return ASM;
    }
    if(node.is_call_assign()){
        ASM += node.get_operand(0)->get_val()->get_node()->accept(*this);
    }
    else{
        ASM += node.get_operand(0)->get_val()->get_node()->accept(*this);
        ASM += node.get_operand(1)->get_val()->get_node()->accept(*this);
        //也有可能是sub，要加入一个op类型的判断
        ASM += "\tadd\ta0, a1, a2\n";
        ASM += "\tsw\ta0,";
        ASM += "-";
        ASM += std::to_string(node.get_stack_addr());
        ASM += "(s0)\n";
        if(node.get_num_users() != 0){          //如果不是立马被使用，这里就不能马上lw，要到合适的时机lw
            ASM += "\tlw\t";
            ASM += "a";
            ASM += std::to_string(reg_cnt);
            ASM += ", ";
            ASM += "-";
            ASM += std::to_string(node.get_stack_addr());
            ASM += "(s0)";
            ASM += "\n";
            reg_cnt++;
        if(reg_cnt == 3) reg_cnt = 1;
        }
    }
    return ASM;
}

std::string AsmPrinter::visit(PrologSDNode &node){
    std::string ASM;
    ASM += node.get_func_name() + ":\n";
    ASM += "\taddi\tsp, sp, -";
    ASM += std::to_string(node.get_frame_size());
    ASM += "\n";
    ASM += "\tsw\tra, ";
    ASM += std::to_string(node.get_frame_size()-4);
    ASM += "(sp)\n";
    ASM += "\tsw\ts0, ";
    ASM += std::to_string(node.get_frame_size()-8);
    ASM += "(sp)\n";
    ASM += "\taddi\ts0, sp,";
    ASM += std::to_string(node.get_frame_size());
    ASM += "\n";

    if(node.get_arg_num() != 0){
        for(int i = 0; i < node.get_arg_num(); i++){
            int arg_addr = 12 + i*4;
            ASM += "\tsw\t";
            ASM += "a";
            ASM += std::to_string(i);
            ASM += ", -";
            ASM += std::to_string(arg_addr);
            ASM += "(s0)\n";
        }
    }
    ASM += "\n";

    if(node.get_func_name() == "output"){
        ASM += "\tlw\ta0,-12(s0)\n";
        ASM += "\tli\ta7, 1\n";
        ASM += "\tecall\n";
        ASM += "\n";
        ASM += "\tlw\ts0, ";
        ASM += std::to_string(node.get_frame_size()-8);
        ASM += "(sp)\n";
        ASM += "\tlw\tra, ";
        ASM += std::to_string(node.get_frame_size()-4);
        ASM += "(sp)\n";
        ASM += "\taddi\tsp, sp, ";
        ASM += std::to_string(node.get_frame_size());
        ASM += "\n";
        ASM += "\tret\n";
    }

    return ASM;
}

std::string AsmPrinter::visit(RootSDNode &node){
    std::string ASM;
    ASM += node.get_chain_depen()->accept(*this);
    return ASM;
}

std::string AsmPrinter::visit(CallSDNode &node){
    std::string ASM;
    ASM += node.get_chain_depen()->accept(*this);
    for(int i = 0; i < node.get_arg_num(); i++){
        ASM += "\tlw\t";
        ASM += "a";
        ASM += std::to_string(i);
        ASM += ", -";
        ASM += std::to_string(node.get_argi_addr(i));
        ASM += "(s0)\n";
    }
    ASM += "\tcall\t";
    ASM += node.get_func_name();
    ASM += "\n";
    if(node.get_num_users() != 0){
        ASM += "\tsw\ta0, -";
        ASM += std::to_string(node.get_ret_user_addr());
        ASM += "(s0)\n";
        //sw a0, ...
    }
    return ASM;
}

std::string AsmPrinter::visit(ReturnSDNode &node){
    std::string ASM;
    ASM += node.get_chain_depen()->accept(*this);
    ASM += "\n";
    if(node.get_has_ret_val()){
        ASM += "\tlw\ta0, -";
        ASM += std::to_string(node.get_ret_val_addr());
        ASM += "(s0)\n";
    }
    ASM += "\tlw\ts0, ";
    ASM += std::to_string(node.get_frame_size()-8);
    ASM += "(sp)\n";
    ASM += "\tlw\tra, ";
    ASM += std::to_string(node.get_frame_size()-4);
    ASM += "(sp)\n";
    ASM += "\taddi\tsp, sp, ";
    ASM += std::to_string(node.get_frame_size());
    ASM += "\n";
    if(node.get_should_exit() == true){
        ASM +=  "\tli\ta7, 10\n";
        ASM +=  "\tecall\n";
    }
    ASM += "\tret\n";
    return ASM;
}