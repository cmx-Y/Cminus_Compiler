#include "BasicBlock.h"
#include "Constant.h"
#include "Function.h"
#include "IRBuilder.h"
#include "Module.h"
#include "Type.h"

#include <iostream>
#include <memory>

#ifdef DEBUG  // 用于调试信息,大家可以在编译过程中通过" -DDEBUG"来开启这一选项
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl;  // 输出行号的简单示例
#else
#define DEBUG_OUTPUT
#endif

#define CONST_INT(num) \
    ConstantInt::get(num, module)

#define CONST_FP(num) \
    ConstantFP::get(num, module) // 得到常数值的表示,方便后面多次用到

int main() {
	auto module = new Module("Cminus code");  // module name是什么无关紧要
	auto builder = new IRBuilder(nullptr, module);
	Type *Int32Type = Type::get_int32_type(module);
	
	//create main function and BasicBlock
	auto mainFun = Function::create(FunctionType::get(Int32Type, {}),"main", module);
	auto bb = BasicBlock::create(module, "entry", mainFun);
  	builder->set_insert_point(bb);
  	
  	//for main default return 0
  	auto retAlloca = builder->create_alloca(Int32Type);
  	builder->create_store(CONST_INT(0), retAlloca);             

	//create array a[10]
	auto *arrayType = ArrayType::get(Int32Type, 10);
	auto a = builder->create_alloca(arrayType);

	//get a[0] a[1] address
	auto a0GEP = builder->create_gep(a, {CONST_INT(0), CONST_INT(0)});  
	builder->create_store(CONST_INT(10), a0GEP);
	auto a1GEP = builder->create_gep(a, {CONST_INT(0), CONST_INT(1)});
	
	auto a0Load = builder->create_load(a0GEP);
	auto mul = builder->create_imul(a0Load, CONST_INT(2));
	builder->create_store(mul, a1GEP);
	auto a1Load = builder->create_load(a1GEP);
	builder->create_ret(a1Load);
	
	std::cout << module->print();
  	delete module;
  	return 0;
}
