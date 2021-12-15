#include "ConstPropagation.hpp"
#include "logging.hpp"


// 给出了返回整形值的常数折叠实现，大家可以参考，在此基础上拓展
// 当然如果同学们有更好的方式，不强求使用下面这种方式
ConstantInt *ConstFolder::compute(
    Instruction::OpID op,
    ConstantInt *value1,
    ConstantInt *value2)
{

    int c_value1 = value1->get_value();
    int c_value2 = value2->get_value();
    switch (op)
    {
    case Instruction::add:
        return ConstantInt::get(c_value1 + c_value2, module_);
        break;
    case Instruction::sub:
        return ConstantInt::get(c_value1 - c_value2, module_);
        break;
    case Instruction::mul:
        return ConstantInt::get(c_value1 * c_value2, module_);
        break;
    case Instruction::sdiv:
        return ConstantInt::get((int)(c_value1 / c_value2), module_);
        break;
    default:
        return nullptr;
        break;
    }
}

ConstantFP *ConstFolder::fcompute(
    Instruction::OpID op,
    ConstantFP *value1,
    ConstantFP *value2
){
    float c_value1 = value1->get_value();
    float c_value2 = value2->get_value();
    switch(op)
    {
    case Instruction::fadd:
        return ConstantFP::get(c_value1 + c_value2, module_);
        break;
    case Instruction::fsub:
        return ConstantFP::get(c_value1 - c_value2, module_);
        break;
    case Instruction::fmul:
        return ConstantFP::get(c_value1 * c_value2, module_);
        break;
    case Instruction::fdiv:
        return ConstantFP::get((int)(c_value1 / c_value2), module_);
        break;
    default:
        return nullptr;
        break;
    }
}

ConstantInt *ConstFolder::fticompute(
    Instruction::OpID op,
    ConstantFP *value1
){
    float c_value1 = value1->get_value();
    if(op == Instruction::fptosi)
        return ConstantInt::get((int)c_value1, module_);
    else
        return nullptr;
}

ConstantFP *ConstFolder::itfcompute(
    Instruction::OpID op,
    ConstantInt *value1
){
    int c_value1 = value1->get_value();
    if(op == Instruction::sitofp)
        return ConstantFP::get((float)c_value1, module_);
    else
        return nullptr;
}

ConstantInt *ConstFolder::zextcompute(
    Instruction::OpID op,
    ConstantInt *value1
){
    int c_value1 = value1->get_value();
    if(op == Instruction::zext)
        return ConstantInt::get((bool)c_value1, module_);
    else
        return nullptr;
}

ConstantInt *ConstFolder::cmpcompute(
    CmpInst::CmpOp op,
    ConstantInt *value1,
    ConstantInt *value2
){
    int c_value1 = value1->get_value();
    int c_value2 = value2->get_value();
    switch(op)
    {
    case CmpInst::EQ:
        return ConstantInt::get(c_value1 == c_value2, module_);
        break;
    case CmpInst::NE:
        return ConstantInt::get(c_value1 != c_value2, module_);
        break;
    case CmpInst::GT:
        return ConstantInt::get(c_value1 > c_value2, module_);
        break;
    case CmpInst::GE:
        return ConstantInt::get(c_value1 >= c_value2, module_);
        break;
    case CmpInst::LT:
        return ConstantInt::get(c_value1 < c_value2, module_);
        break;
    case CmpInst::LE:
        return ConstantInt::get(c_value1 <= c_value2, module_);
        break;
    default:
        return nullptr;
        break;
    }
}

ConstantInt *ConstFolder::fcmpcompute(
    FCmpInst::CmpOp op,
    ConstantFP *value1,
    ConstantFP *value2
){
    float c_value1 = value1->get_value();
    float c_value2 = value2->get_value();
    switch(op)
    {
    case FCmpInst::EQ:
        return ConstantInt::get(c_value1 == c_value2, module_);
        break;
    case FCmpInst::NE:
        return ConstantInt::get(c_value1 != c_value2, module_);
        break;
    case FCmpInst::GT:
        return ConstantInt::get(c_value1 > c_value2, module_);
        break;
    case FCmpInst::GE:
        return ConstantInt::get(c_value1 >= c_value2, module_);
        break;
    case FCmpInst::LT:
        return ConstantInt::get(c_value1 < c_value2, module_);
        break;
    case FCmpInst::LE:
        return ConstantInt::get(c_value1 <= c_value2, module_);
        break;
    default:
        return nullptr;
        break;
    }
}

// 用来判断value是否为ConstantFP，如果不是则会返回nullptr
ConstantFP *cast_constantfp(Value *value)
{
    auto constant_fp_ptr = dynamic_cast<ConstantFP *>(value);
    if (constant_fp_ptr)
    {
        return constant_fp_ptr;
    }
    else
    {
        return nullptr;
    }
}
ConstantInt *cast_constantint(Value *value)
{
    auto constant_int_ptr = dynamic_cast<ConstantInt *>(value);
    if (constant_int_ptr)
    {
        return constant_int_ptr;
    }
    else
    {
        return nullptr;
    }
}


void ConstPropagation::run()
{
    // 从这里开始吧！
    auto c = ConstFolder(m_);
    auto func_list = m_->get_functions();
    std::vector<Instruction *> instodelete;
    
    std::map<std::string, Constant *> const_map;
    std::map<std::string, Constant *>::iterator k;
    auto is_const_value = true;
    for (auto func : func_list)
    {
        if (func->get_basic_blocks().size() == 0)
        {
            continue;
        }
        else{
            for(auto bb: func->get_basic_blocks()){                             
                std::map<std::string, Constant *> const_vector;     
                for(auto ins: bb->get_instructions()){
                    if(is_const_value == false)
                        is_const_value = true;
                    if(ins->get_num_operand() <= 0){     
                        continue;
                    }
                    for(auto i = 0; i < ins->get_num_operand() && is_const_value == true; i++){
                        auto operand = ins->get_operand(i);
                        if(cast_constantfp(operand) == nullptr && cast_constantint(operand) == nullptr){
                            is_const_value = false;
                        }
                    }
                    
                    if(is_const_value){
                        Constant* const_value = nullptr;
                        if(ins->is_add() || ins->is_sub() || ins->is_mul() || ins->is_div())
                            const_value = c.compute(ins->get_instr_type(), cast_constantint(ins->get_operand(0)),cast_constantint(ins->get_operand(1))); 
                        else if(ins->is_fadd() || ins->is_fsub() || ins->is_fmul() || ins->is_fdiv())
                            const_value = c.fcompute(ins->get_instr_type(), cast_constantfp(ins->get_operand(0)),cast_constantfp(ins->get_operand(1)));
                        else if(ins->is_cmp()){
                            auto ins_cmp = dynamic_cast<CmpInst*>(ins);
                            const_value = c.cmpcompute(ins_cmp->get_cmp_op(), cast_constantint(ins->get_operand(0)),cast_constantint(ins->get_operand(1)));
                        }         
                        else if(ins->is_fcmp()){
                            auto ins_fcmp = dynamic_cast<FCmpInst*>(ins);
                            const_value = c.fcmpcompute(ins_fcmp->get_cmp_op() , cast_constantfp(ins->get_operand(0)),cast_constantfp(ins->get_operand(1)));
                        }   
                        else if(ins->is_fp2si())
                            const_value = c.fticompute(ins->get_instr_type(), cast_constantfp(ins->get_operand(0)));
                        else if(ins->is_si2fp())
                            const_value = c.itfcompute(ins->get_instr_type(), cast_constantint(ins->get_operand(0)));
                        else if(ins->is_zext())
                            const_value = c.zextcompute(ins->get_instr_type(), cast_constantint(ins->get_operand(0)));
                        if(const_value != nullptr){
                            ins->remove_use_of_ops();
                            ins->replace_all_use_with(const_value);
                            //bb->delete_instr(ins);            can't delete immediately!!  segmentation fault!!
                            instodelete.push_back(ins);
                            const_vector.insert({ins->get_name(), const_value});
                        }
                    }

                    else if(ins->is_br()){
                        if(ins->get_num_operand() <= 1)
                            continue;
                        auto cont = cast_constantint(ins->get_operand(0));
                        auto is_const = (const_vector.find(ins->get_operand(0)->get_name()) == const_vector.end()) ? 0 : 1;
                        if(cont){
                            auto br1 = cont->get_value() > 0 ? 1 : 2;
                            auto br2 = br1 == 1 ? 2 : 1;
                            auto BB1 = ins->get_operand(br1);
                            auto BB2 = dynamic_cast<BasicBlock*> (ins->get_operand(br2));
                            ins->remove_operands(0, 2);
                            ins->add_operand(BB1);
                            ins->add_use(BB1);
                            bb->remove_succ_basic_block(BB2);
                            BB2->remove_pre_basic_block(bb);
                        }
                        else if(is_const){
                            
                            auto find_map = const_vector.find(ins->get_operand(0)->get_name());
                            auto br1 = dynamic_cast<ConstantInt *>(find_map->second)->get_value() > 0 ? 1 : 2;
                            
                            auto br2 = br1 == 1 ? 2 : 1;
                            auto BB1 = ins->get_operand(br1);
                            auto BB2 = dynamic_cast<BasicBlock*> (ins->get_operand(br2));
                            ins->remove_operands(0, 2);
                            ins->add_operand(BB1);
                            ins->add_use(BB1);
                            bb->remove_succ_basic_block(BB2);
                            BB2->remove_pre_basic_block(bb);
                        }
                    }
                    
                    else if(ins->is_store()){
                        Constant* store_value;
                        auto is_const = false; 
                        if(cast_constantfp(ins->get_operand(0))){
                            is_const = true;
                            store_value = cast_constantfp(ins->get_operand(0));
                        }
                        else if(cast_constantint(ins->get_operand(0))){
                            is_const = true;
                            store_value = cast_constantint(ins->get_operand(0));
                        }
                        if(is_const){
                            const_vector.insert({ins->get_operand(1)->get_name(), store_value});
                        }
                    }
                    else if(ins->is_load()){
                        auto n = const_vector.find(ins->get_operand(0)->get_name());
                        if(n != const_vector.end()){
                            ins->remove_use_of_ops();
                            ins->replace_all_use_with(n->second);
                            instodelete.push_back(ins);
                        }
                    }       
                }
                for(auto ins: instodelete){
                    bb->delete_instr(ins);
                }
            }    
        }
    }
}