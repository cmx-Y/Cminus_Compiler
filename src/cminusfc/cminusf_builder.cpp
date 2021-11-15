#include "cminusf_builder.hpp"


// use these macros to get constant value
#define CONST_FP(num) \
    ConstantFP::get((float)num, module.get())
#define CONST_ZERO(type) \
    ConstantZero::get(type, module.get())
#define CONST_INT(num) \
    ConstantInt::get(num, module.get())


// You can define global variables here
// to store state
Value *cur_arg;
std::vector<Function> AllFun;   //vector of all function, used for call
bool isleftvalue;
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
    type = node.type;                                                   //type is enum type 
    switch (node.type)                                                  //get num value by type
    {
    case TYPE_INT:                                                      
        val = CONST_INT(node.i_val);                                    
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
    scope.push(node.id, var);
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
    AllFun.push_back(*Fun);                                             //push function to AllFun

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

void CminusfBuilder::visit(ASTExpressionStmt &node) {
    node.expression->accept(*this);

 }

void CminusfBuilder::visit(ASTSelectionStmt &node) { }

void CminusfBuilder::visit(ASTIterationStmt &node) { }

void CminusfBuilder::visit(ASTReturnStmt &node) {
    if(node.expression == nullptr)
        builder->create_void_ret();
        
 }

void CminusfBuilder::visit(ASTVar &node) { 
    auto var = scope.find(node.id);
    auto TyInt32 = Type::get_int32_type(module.get());
    auto TyFloat = Type::get_float_type(module.get());

    if(node.expression == nullptr){
        if(isleftvalue){                                    //there's no value in the alloca, should store first
            val = var;
            isleftvalue = false;
            type = val->get_type()->get_pointer_element_type()==TyInt32 ? TYPE_INT : TYPE_FLOAT;
        }
        else{
            val = builder->create_load(var);
            type = val->get_type()==TyInt32 ? TYPE_INT : TYPE_FLOAT;             
        }
    }
    else{
        node.expression->accept(*this);
        auto expr_val = val;
        auto expr_type = type;
        auto isnegBB = BasicBlock::create(module.get(), "", builder->get_insert_block()->get_parent());
        auto notnegBB = BasicBlock::create(module.get(), "", builder->get_insert_block()->get_parent());
        if(expr_type == TYPE_FLOAT){
            expr_val = FpToSiInst::create_fptosi(expr_val, TyInt32, builder->get_insert_block()); 
        }
        auto isneg = builder->create_icmp_lt(expr_val, CONST_INT(0));
        
        auto br = builder->create_cond_br(isneg, isnegBB, notnegBB);
        builder->set_insert_point(isnegBB);
        builder->create_call(scope.find("neg_idx_except"), {});
        builder->create_br(notnegBB);
        /*if (builder->get_insert_block()->get_parent()->get_return_type()->is_void_type())
            builder->create_void_ret();
        else if (builder->get_insert_block()->get_parent()->get_return_type()->is_float_type())
            builder->create_ret(CONST_FP(0.));
        else
            builder->create_ret(CONST_INT(0));*/

        builder->set_insert_point(notnegBB);
        var = builder->create_gep(var, {CONST_INT(0), expr_val});

        if(isleftvalue){                                    //there's no value in the alloca, should store first
            val = var;
            isleftvalue = false;
            type = val->get_type()->get_pointer_element_type()==TyInt32 ? TYPE_INT : TYPE_FLOAT;
        }
        else{
            val = builder->create_load(var);  
            type = val->get_type()==TyInt32 ? TYPE_INT : TYPE_FLOAT;           
        }
    }
}

void CminusfBuilder::visit(ASTAssignExpression &node) { 
    auto TyInt32 = Type::get_int32_type(module.get());
    auto TyFloat = Type::get_float_type(module.get());
    isleftvalue = true;
    node.expression->accept(*this);                         
    auto expr_val = val;
    auto expr_type = type;
    node.var->accept(*this);
    auto var_val = val;
    auto var_type = type;
    if(var_type != expr_type){                              
        if(var_type == TYPE_INT)
            expr_val = FpToSiInst::create_fptosi(expr_val, TyInt32, builder->get_insert_block());
        else if(var_type == TYPE_FLOAT)
            expr_val = SiToFpInst::create_sitofp(expr_val, TyInt32, builder->get_insert_block());
    }
    builder->create_store(expr_val, var_val);
    val = expr_val;
}


void CminusfBuilder::visit(ASTSimpleExpression &node) {     //code similar to belows
    auto TyInt32 = Type::get_int32_type(module.get());
    auto TyFloat = Type::get_float_type(module.get());
    if(node.additive_expression_r == nullptr)               //attention that additive_expression_r may be nullptr!
        node.additive_expression_l->accept(*this);
    else{
        node.additive_expression_l->accept(*this);
        auto val_l = val;
        node.additive_expression_r->accept(*this);
        auto val_r = val;
        auto is_int = false;
        if(val_l->get_type()->is_integer_type() && val_r->get_type()->is_integer_type() )
            is_int = true;
        Value* cmp;
        if(is_int){
            switch (node.op){
                case OP_LE:
                    cmp = builder->create_icmp_le(val_l, val_r);
                    break;
                case OP_LT:
                    cmp = builder->create_icmp_lt(val_l, val_r);
                    break;
                case OP_EQ:
                    cmp = builder->create_icmp_eq(val_l, val_r);
                    break;
                case OP_NEQ:
                    cmp = builder->create_icmp_ne(val_l, val_r);
                    break;
                case OP_GE:
                    cmp = builder->create_icmp_ge(val_l, val_r);
                    break;
                case OP_GT:
                    cmp = builder->create_icmp_gt(val_l, val_r);
                    break;
            }
        }
        else {
            if(val_l->get_type()->is_integer_type())
                    val_l = SiToFpInst::create_sitofp(val_l, TyFloat, builder->get_insert_block());
            else if(val_r->get_type()->is_integer_type())
                    val_r = SiToFpInst::create_sitofp(val_r, TyFloat, builder->get_insert_block());
            switch (node.op){
                case OP_LE:
                    cmp = builder->create_fcmp_le(val_l, val_r);
                    break;
                case OP_LT:
                    cmp = builder->create_fcmp_lt(val_l, val_r);
                    break;
                case OP_EQ:
                    cmp = builder->create_fcmp_eq(val_l, val_r);
                    break;
                case OP_NEQ:
                    cmp = builder->create_fcmp_ne(val_l, val_r);
                    break;
                case OP_GE:
                    cmp = builder->create_fcmp_ge(val_l, val_r);
                    break;
                case OP_GT:
                    cmp = builder->create_fcmp_gt(val_l, val_r);
                    break;
            }
        }
        type = TYPE_INT;
        val = builder->create_zext(cmp, Type::get_int32_type(module.get()));
    }
 }

void CminusfBuilder::visit(ASTAdditiveExpression &node) {
    CminusType l_type, r_type;
    Value *l_val, *r_val;

    auto TyInt32 = Type::get_int32_type(module.get());
    auto TyFloat = Type::get_float_type(module.get());
    //get value and type
    if(node.additive_expression != nullptr){
        node.additive_expression->accept(*this);
        l_val = val;
        l_type = type;
    }
    node.term->accept(*this);
    r_val = val;
    r_type = type;

    //calculate to get new type and val
    if(node.additive_expression == nullptr){
        type = r_type;
        val = r_val;
    }
    else{
        if(l_type == r_type && l_type == TYPE_INT)
            type = TYPE_INT;
        else type = TYPE_FLOAT;
        if(type == TYPE_INT){
            switch (node.op) {
            case OP_PLUS:
                val = builder->create_iadd(l_val, r_val);
                break;
            case OP_MINUS:
                val = builder->create_isub(l_val, r_val);
                break;
            }
        }
        else{
            if(l_type == TYPE_INT)
                    l_val = SiToFpInst::create_sitofp(l_val, TyFloat, builder->get_insert_block());
            else if(r_type == TYPE_INT)
                    r_val = SiToFpInst::create_sitofp(r_val, TyFloat, builder->get_insert_block());
            switch (node.op) {
            case OP_PLUS:
                val = builder->create_fadd(l_val, r_val);
                break;
            case OP_MINUS:
                val = builder->create_fsub(l_val, r_val);
                break;
            }
        }
    }
 }

void CminusfBuilder::visit(ASTTerm &node) {
    CminusType l_type, r_type;
    Value *l_val, *r_val;

    auto TyInt32 = Type::get_int32_type(module.get());
    auto TyFloat = Type::get_float_type(module.get());
    if(node.term != nullptr){
        node.term->accept(*this);
        l_val = val;
        l_type = type;
    }
    node.factor->accept(*this);
    r_val = val;
    r_type = type;
    if(node.term == nullptr){
        type = r_type;
        val = r_val;
    }
    else{
        if(l_type == r_type && l_type == TYPE_INT)
            type = TYPE_INT;
        else type = TYPE_FLOAT;
        if(type == TYPE_INT){
            switch (node.op) {
            case OP_MUL:
                val = builder->create_imul(l_val, r_val);
                break;
            case OP_DIV:
                val = builder->create_isdiv(l_val, r_val);
                break;
            }
        }
        else{
            if(l_type == TYPE_INT)
                    l_val = SiToFpInst::create_sitofp(l_val, TyFloat, builder->get_insert_block());
            else if(r_type == TYPE_INT)
                    r_val = SiToFpInst::create_sitofp(r_val, TyFloat, builder->get_insert_block());
            switch (node.op) {
            case OP_MUL:
                val = builder->create_fmul(l_val, r_val);
                break;
            case OP_DIV:
                val = builder->create_fdiv(l_val, r_val);
                break;
            }
        }
    }
 }

void CminusfBuilder::visit(ASTCall &node) {
    auto TyInt32 = Type::get_int32_type(module.get());
    auto TyFloat = Type::get_float_type(module.get());
    auto FunTy = Function::create(AllFun[0].get_function_type(),AllFun[0].get_name(),module.get());
    //Function FunTy(AllFun[0].get_function_type(),AllFun[0].get_name(),module.get());    //default create
    std::vector<Value *> args;
    for(auto cur_Fun = AllFun.begin(); cur_Fun != AllFun.end(); cur_Fun++){             //find the function to be called
        if((*cur_Fun).get_name() == node.id){
            *FunTy = *cur_Fun;
            break;
        }
    }
    for (auto iter = node.args.begin(); iter != node.args.end(); iter++) {
        (*iter)->accept(*this);         //(*) is needed!!!
        if(FunTy->get_name() == "output" && type == TYPE_FLOAT){        //attention that output()'s param type is int!!
            auto fptosi = FpToSiInst::create_fptosi(val, TyInt32, builder->get_insert_block());
            args.push_back(fptosi);
        }
        else args.push_back(val);   
    }
    auto call = builder->create_call(FunTy, args);                                      //what's the reason for segmentation default
    module->pop_function();
 }
