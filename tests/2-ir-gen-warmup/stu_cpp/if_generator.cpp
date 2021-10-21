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
	//set int32 and float type
	auto module = new Module("Cminus code"); 
	auto builder = new IRBuilder(nullptr, module);
	Type *Int32Type = Type::get_int32_type(module);
	Type *floatType = Type::get_float_type(module);
		
	//create main function and BasicBlock
	auto mainFun = Function::create(FunctionType::get(Int32Type, {}),"main", module);
	auto bb = BasicBlock::create(module, "entry", mainFun);
  	builder->set_insert_point(bb);
  	
  	//for main default return 0
  	auto retAlloca = builder->create_alloca(Int32Type);
  	builder->create_store(CONST_INT(0), retAlloca);
  	
  	//create float a = 5.555
  	auto aAlloca = builder->create_alloca(floatType);
  	builder->create_store(CONST_FP(5.555), aAlloca);
  	auto aLoad = builder->create_load(aAlloca);
  	
  	//compare a with 1
  	auto fcmp = builder->create_fcmp_gt(aLoad, CONST_FP(1));  

	//create true, false, return BasicBlocks
  	auto trueBB = BasicBlock::create(module, "trueBB", mainFun);    
  	auto falseBB = BasicBlock::create(module, "falseBB", mainFun);  
  	auto retBB = BasicBlock::create(module, "", mainFun);  
             
	auto br = builder->create_cond_br(fcmp, trueBB, falseBB);
	
	//if true,return 233
	builder->set_insert_point(trueBB);  
  	builder->create_store(CONST_INT(233), retAlloca);
  	builder->create_br(retBB);  // br retBB

	//if false,return default 0
  	builder->set_insert_point(falseBB);  
  	builder->create_br(retBB);

    builder->set_insert_point(retBB);  // ret分支
  	auto retLoad = builder->create_load(retAlloca);
  	builder->create_ret(retLoad);
     
	
	std::cout << module->print();
  	delete module;
  	return 0;
}
