#include "BasicBlock.h"
#include "Constant.h"
#include "Function.h"
#include "IRBuilder.h"
#include "Module.h"
#include "Type.h"

#include <iostream>
#include <memory>

#ifdef DEBUG  
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl;  
#else
#define DEBUG_OUTPUT
#endif

#define CONST_INT(num) \
    ConstantInt::get(num, module)

#define CONST_FP(num) \
    ConstantFP::get(num, module) 

int main() {
	//create module and builder
	auto module = new Module("Cminus code"); 
	auto builder = new IRBuilder(nullptr, module);
	Type *Int32Type = Type::get_int32_type(module);
	
	//create callee function and BasicBlock
  	std::vector<Type *> Ints(1, Int32Type);
  	auto calleeFunTy = FunctionType::get(Int32Type, Ints);
  	auto calleeFun = Function::create(calleeFunTy,"callee", module);
  	auto bb = BasicBlock::create(module, "entry", calleeFun);
  	builder->set_insert_point(bb);
  	
  	//create formal parameters alloca
  	auto retAlloca = builder->create_alloca(Int32Type);   
  	auto aAlloca = builder->create_alloca(Int32Type);

	//get formal parameters of calleeFun
  	std::vector<Value *> args;  
  	for (auto arg = calleeFun->arg_begin(); arg != calleeFun->arg_end(); arg++) {
    	args.push_back(*arg);   
  	}
  	builder->create_store(args[0], aAlloca);
  	
  	//get mul_result
  	auto aLoad = builder->create_load(aAlloca);
  	auto mul = builder->create_imul(aLoad, CONST_INT(2));
	builder->create_ret(mul);


	
	//create main function and BasicBlock
	auto mainFun = Function::create(FunctionType::get(Int32Type, {}),"main", module);
	bb = BasicBlock::create(module, "entry", mainFun);
  	builder->set_insert_point(bb);
  	
  	//for main default return 0
  	retAlloca = builder->create_alloca(Int32Type);
  	builder->create_store(CONST_INT(0), retAlloca);             

	//call calleeFun
	auto call = builder->create_call(calleeFun, {CONST_INT(110)});         
  	builder->create_ret(call);
	
	std::cout << module->print();
  	delete module;
  	return 0;
}
