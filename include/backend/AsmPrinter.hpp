#ifndef SYSYC_ASMPRINTER_HPP
#define SYSYC_ASMPRINTER_HPP

#include "BasicBlock.h"
#include "Module.h"
#include "SelDAGNodes.hpp"

class AsmPrinter : public SDNodeVisitor{
public:
    virtual std::string visit(BinarySDNode &node) override final;
    virtual std::string visit(PrologSDNode &node) override final;
    virtual std::string visit(RootSDNode &node) override final;
    virtual std::string visit(ReturnSDNode &node) override final;
    virtual std::string visit(CallSDNode &node) override final;

private:

};

#endif