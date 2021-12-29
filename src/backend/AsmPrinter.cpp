#include "AsmPrinter.hpp"

void AsmPrinter::visit(BinarySDNode &node){
    if(node.get_num_operands() == 0){
        std::cout << "leaf" << std::endl;
        return;
    }
    node.get_operand(0)->get_val()->get_node()->accept(*this);
    node.get_operand(1)->get_val()->get_node()->accept(*this);
    std::cout << node.get_node_name() << std::endl;
}