#pragma once

#include <unordered_map>
#include <unordered_set>
#include "PassManager.hpp"
#include "Module.h"
#include "Function.h"
#include "BasicBlock.h"

class LoopInvHoist : public Pass {
public:
    LoopInvHoist(Module *m) : Pass(m) {}
    bool is_inv_operand(Value* operand);

    void run() override;
};
