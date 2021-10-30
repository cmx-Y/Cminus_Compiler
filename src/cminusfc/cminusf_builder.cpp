#include "cminusf_builder.hpp"

// use these macros to get constant value
#define CONST_FP(num) \
    ConstantFP::get((float)num, module.get())
#define CONST_ZERO(type) \
    ConstantZero::get(var_type, module.get())


// You can define global variables here
// to store state

/*
 * use CMinusfBuilder::Scope to construct scopes
 * scope.enter: enter a new scope
 * scope.exit: exit current scope
 * scope.push: add a new binding to current scope
 * scope.find: find and return the value bound to the name
 */

void CminusfBuilder::visit(ASTProgram &node) { 
    if(node.declarations.size == 0)                         //没有定义，出错
        exit;
    for (auto cur_declaration : node.declarations)          //遍历node.declarations中的所有元素
    {
        cur_declaration->accept(*this);
    }
}

void CminusfBuilder::visit(ASTNum &node) { 
    switch (node.type)                                      //根据num的类型得到常数的值
    {
    case TYPE_INT:                                          //val定义在CminusfBuilder类中
        val = ConstantInt::get(node.i_val, module.get());   //CONST_INT(node.i_val)
        break;
    case TYPE_FLOAT:
        val = CONST_FP(node.f_val);
        break;
    default:
        exit;
    }
}

void CminusfBuilder::visit(ASTVarDeclaration &node) { 
    auto *vartype, *varTy;
    if(node.type == TYPE_INT)
        vartype = Type::get_int32_type(module.get());
    else if(node.type == TYPE_FLOAT)
        vartype = Type::get_float_type(module.get());
    if(node.num == nullptr)                                     //说明产生式为var-declaration →type-specifier ID ;
        varTy = vartype;                                        
    else                                                        //产生式为type-specifier ID [ INTEGER ] ;
        varTy = ArrayType::get(vartype, node.num);              //定义数组类型
     
    if(builder->get_insert_block){                              //访问者在某个模块中
        auto var = builder->create_alloca(varTy);
    }
    else{                                                       //全局变量
        auto var = GlobalVariable::create(node.id, module.get(), varTy, false, CONST_ZERO(varTy));
    }    
    
    scope.push(node.id, var);
}

void CminusfBuilder::visit(ASTFunDeclaration &node) { 
    auto *retTy;                                                //返回值类型
    if(node.type == TYPE_INT)
        retTy = Type::get_int32_type(module.get());
    else if(node.type == TYPE_FLOAT)
        retTy = Type::get_float_type(module.get());
    else
        retTy = Type::get_void_type(module.get());
    
    //为了定义函数类型，需要先得到参数类型列表
    for()
}

void CminusfBuilder::visit(ASTParam &node) { }

void CminusfBuilder::visit(ASTCompoundStmt &node) { }

void CminusfBuilder::visit(ASTExpressionStmt &node) { }

void CminusfBuilder::visit(ASTSelectionStmt &node) { }

void CminusfBuilder::visit(ASTIterationStmt &node) { }

void CminusfBuilder::visit(ASTReturnStmt &node) { }

void CminusfBuilder::visit(ASTVar &node) { }

void CminusfBuilder::visit(ASTAssignExpression &node) { }

void CminusfBuilder::visit(ASTSimpleExpression &node) { }

void CminusfBuilder::visit(ASTAdditiveExpression &node) { }

void CminusfBuilder::visit(ASTTerm &node) { }

void CminusfBuilder::visit(ASTCall &node) { }
