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
	
	
	//create main function and BasicBlock
	auto mainFun = Function::create(FunctionType::get(Int32Type, {}),"main", module);
	auto bb = BasicBlock::create(module, "entry", mainFun);
  	builder->set_insert_point(bb);
  	
  	//for main default return 0
  	auto retAlloca = builder->create_alloca(Int32Type);
  	builder->create_store(CONST_INT(0), retAlloca);             

	//create a, i and assign 
	auto aAlloca = builder->create_alloca(Int32Type);
	auto iAlloca = builder->create_alloca(Int32Type);
	builder->create_store(CONST_INT(10), aAlloca);
	builder->create_store(CONST_INT(0), iAlloca);
	
	auto aLoad = builder->create_load(aAlloca);
	auto iLoad = builder->create_load(iAlloca);
	
	//create true, false, BasicBlocks
  	auto trueBB = BasicBlock::create(module, "trueBB", mainFun);    
  	auto falseBB = BasicBlock::create(module, "falseBB", mainFun);
  	
  	auto icmp = builder->create_icmp_lt(iLoad, CONST_INT(10));
  	auto br = builder->create_cond_br(icmp, trueBB, falseBB);
  	
  	//if true
  	//i = i + 1
	builder->set_insert_point(trueBB);  
	iLoad = builder->create_load(iAlloca);
	auto iadd = builder->create_iadd(iLoad, CONST_INT(1));
	builder->create_store(iadd, iAlloca);
	iLoad = builder->create_load(iAlloca);
	
	//a = a + i
	aLoad = builder->create_load(aAlloca);
	auto aadd = builder->create_iadd(aLoad, iLoad);
	builder->create_store(aadd, aAlloca);
	
	icmp = builder->create_icmp_lt(iLoad, CONST_INT(10));
  	br = builder->create_cond_br(icmp, trueBB, falseBB);

	//if false,return a
  	builder->set_insert_point(falseBB);
  	auto retLoad = builder->create_load(aAlloca); 
  	builder->create_ret(retLoad);

	
	std::cout << module->print();
  	delete module;
  	return 0;
}
