#include "cminusf_builder.hpp"


// use these macros to get constant value
#define CONST_FP(num) \
    ConstantFP::get((float)num, module.get())
#define CONST_ZERO(type) \
    ConstantZero::get(type, module.get())


// You can define global variables here
// to store state
Value *cur_arg;
/*
 * use CMinusfBuilder::Scope to construct scopes
 * scope.enter: enter a new scope
 * scope.exit: exit current scope
 * scope.push: add a new binding to current scope
 * scope.find: find and return the value bound to the name
 */

void CminusfBuilder::visit(ASTProgram &node) { 
    if(node.declarations.size() == 0)                                   //no declarations
        exit;
    for (auto& cur_declaration : node.declarations)                      //travel node.declarations
    {
        cur_declaration->accept(*this);
    }
}

void CminusfBuilder::visit(ASTNum &node) { 
    switch (node.type)                                                  //get num value by type
    {
    case TYPE_INT:                                                      
        val = ConstantInt::get(node.i_val, module.get());               //CONST_INT(node.i_val)
        break;
    case TYPE_FLOAT:
        val = CONST_FP(node.f_val);
        break;
    default:
        exit;
    }
}

void CminusfBuilder::visit(ASTVarDeclaration &node) { 
    Type *vartype, *varTy;
    if(node.type == TYPE_INT)
        vartype = Type::get_int32_type(module.get());
    else if(node.type == TYPE_FLOAT)
        vartype = Type::get_float_type(module.get());
    if(node.num == nullptr)                                             
        varTy = vartype;                                        
    else                                                                
        varTy = ArrayType::get(vartype, node.num->i_val);               //array type IR
    
    Value *var; 
    if(!scope.in_global()){                                             //local variable
        var = builder->create_alloca(varTy);
    }
    else{                                                               //global variable
        var = GlobalVariable::create(node.id, module.get(), varTy, false, CONST_ZERO(varTy));
    }    
    
}

void CminusfBuilder::visit(ASTFunDeclaration &node) { 
    Type *retTy, *paramTy;                                              //return type
    std::vector<Type*>  paramTypes;

    if(node.type == TYPE_INT)
        retTy = Type::get_int32_type(module.get());
    else if(node.type == TYPE_FLOAT)
        retTy = Type::get_float_type(module.get());
    else
        retTy = Type::get_void_type(module.get());
    
    //get parameter type list to define function type
    for(auto cur_param : node.params){
        if(!cur_param->isarray){
            if (cur_param->type == TYPE_INT)
                paramTy = Type::get_int32_type(module.get());
            else if(cur_param->type == TYPE_FLOAT)
                paramTy = Type::get_float_type(module.get());
        }
        else{
            if (cur_param->type == TYPE_INT)
                paramTy = Type::get_int32_ptr_type(module.get());
            else if(cur_param->type == TYPE_FLOAT)
                paramTy = Type::get_float_ptr_type(module.get());
        }
        paramTypes.push_back(paramTy);                                  //add paramTy to paramTypes
    }

    //use paramTypes and retTy to define FunctionType
    auto FunTy = FunctionType::get(retTy, paramTypes);

    //define Function according FunctionType
    auto Fun = Function::create(FunTy, node.id, module.get());

    scope.push(node.id, Fun);
    //enter a new scope for Fun
    scope.enter();

    auto bb = BasicBlock::create(module.get(), "entry", Fun);
    builder->set_insert_point(bb);
    std::vector<Value *> args;                                          // 获取gcd函数的形叄1�7,通过Function中的iterator
    for (auto arg = Fun->arg_begin(); arg != Fun->arg_end(); arg++) {
        args.push_back(*arg);                                           // * 号运算符是从迭代器中取出迭代器当前指向的元素
    }
    auto i = 0;
    for(auto cur_param : node.params){
        cur_arg = args[i++];
        cur_param->accept(*this);
    }

    node.compound_stmt->accept(*this);
    scope.exit();   
}

void CminusfBuilder::visit(ASTParam &node) { 
    Type *paramTy;
    if(!node.isarray){
        if(node.type == TYPE_INT)
            paramTy = Type::get_int32_type(module.get());
        else if(node.type == TYPE_FLOAT)
            paramTy = Type::get_float_type(module.get());
    }
    else{
        if(node.type == TYPE_INT)
            paramTy = Type::get_int32_ptr_type(module.get());
        else if(node.type == TYPE_FLOAT)
            paramTy = Type::get_float_ptr_type(module.get());
    }

    auto paramAlloca = builder->create_alloca(paramTy);
    builder->create_store(cur_arg, paramAlloca);
    scope.push(node.id, paramAlloca);
}

void CminusfBuilder::visit(ASTCompoundStmt &node) {
    for(auto cur_local_declaration : node.local_declarations){
        cur_local_declaration->accept(*this);
    }
    for(auto cur_statement : node.statement_list){
        cur_statement->accept(*this);
    }
 }

void CminusfBuilder::visit(ASTExpressionStmt &node) { }

void CminusfBuilder::visit(ASTSelectionStmt &node) { }

void CminusfBuilder::visit(ASTIterationStmt &node) { }

void CminusfBuilder::visit(ASTReturnStmt &node) {
    if(node.expression == nullptr)
        builder->create_void_ret();    
 }

void CminusfBuilder::visit(ASTVar &node) { }

void CminusfBuilder::visit(ASTAssignExpression &node) { }

void CminusfBuilder::visit(ASTSimpleExpression &node) { }

void CminusfBuilder::visit(ASTAdditiveExpression &node) { }

void CminusfBuilder::visit(ASTTerm &node) { }

void CminusfBuilder::visit(ASTCall &node) { }
