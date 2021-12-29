#ifndef SYSYC_ASMPRINTER_HPP
#define SYSYC_ASMPRINTER_HPP

#include "BasicBlock.h"
#include "Module.h"
#include "SelDAGNodes.hpp"

class AsmPrinter : public SDNodeVisitor{
public:
    virtual void visit(BinarySDNode &node) override final;

private:

};

#endif