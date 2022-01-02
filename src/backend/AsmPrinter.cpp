#include "AsmPrinter.hpp"

std::string AsmPrinter::visit(BinarySDNode &node){
    std::string ASM;
    if(node.get_num_operands() == 0){
        ASM += "leaf\n";
        return ASM;
    }
    ASM += node.get_operand(0)->get_val()->get_node()->accept(*this);
    ASM += node.get_operand(1)->get_val()->get_node()->accept(*this);
    ASM += node.get_node_name() + "\n";
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
    return ASM;
}

std::string AsmPrinter::visit(RootSDNode &node){
    std::string ASM;
    ASM += node.get_chain_depen()->accept(*this);
    return ASM;
}

std::string AsmPrinter::visit(ReturnSDNode &node){
    std::string ASM;
    ASM += node.get_chain_depen()->accept(*this);
    ASM += "ret\n";
    return ASM;
}