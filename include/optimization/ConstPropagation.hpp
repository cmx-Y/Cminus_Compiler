#ifndef CONSTPROPAGATION_HPP
#define CONSTPROPAGATION_HPP
#include "PassManager.hpp"
#include "Constant.h"
#include "Instruction.h"
#include "Module.h"

#include "Value.h"
#include "IRBuilder.h"
#include <vector>
#include <stack>
#include <unordered_map>

// tips: 用来判断value是否为ConstantFP/ConstantInt
ConstantFP* cast_constantfp(Value *value);
ConstantInt* cast_constantint(Value *value);


// tips: ConstFloder类

class ConstFolder
{
public:
    ConstFolder(Module *m) : module_(m) {}
    ConstantInt *compute(
        Instruction::OpID op,
        ConstantInt *value1,
        ConstantInt *value2);
    ConstantFP *fcompute(
        Instruction::OpID op,
        ConstantFP *value1,
        ConstantFP *value2
    );
    ConstantInt *fticompute(
        Instruction::OpID op,
        ConstantFP *value1
    );
    ConstantFP *itfcompute(
        Instruction::OpID op,
        ConstantInt *value1
    );
    ConstantInt *zextcompute(
        Instruction::OpID op,
        ConstantInt *value1
    );
    ConstantInt *cmpcompute(
        CmpInst::CmpOp op,
        ConstantInt *value1,
        ConstantInt *value2
    );
    ConstantInt *fcmpcompute(
        FCmpInst::CmpOp op,
        ConstantFP *value1,
        ConstantFP *value2
    );
private:
    Module *module_;
};

class ConstPropagation : public Pass
{
public:
    ConstPropagation(Module *m) : Pass(m) {}
    void visit_bb(BasicBlock* bb);
    void run();
};

#endif
