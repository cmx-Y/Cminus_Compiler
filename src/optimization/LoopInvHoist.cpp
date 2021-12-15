#include <algorithm>
#include "logging.hpp"
#include "LoopSearch.hpp"
#include "LoopInvHoist.hpp"

void LoopInvHoist::run()
{
    //get messages by LoopSearch    
    LoopSearch loop_searcher(m_, false);
    loop_searcher.run();
    std::map<std::string, BasicBlock *> varname2bb;

    auto func_list = this->m_->get_functions();
    for(auto func : func_list)
    {
        func->set_instr_name();
        if(func->get_basic_blocks().size() == 0)
        {
            continue;
        }
        else
        {
            for(auto bb: func->get_basic_blocks())
                for(auto instr : bb->get_instructions())
                        varname2bb[instr->get_name()] = instr->get_parent();

            auto loops = loop_searcher.get_loops_in_func(func);
            for(auto loop : loops)
            {
                BasicBlock* target_prev;    //special situation: if succ is also while base
                auto loop_base_bb = loop_searcher.get_loop_base(loop);
                for(auto prev : loop_base_bb->get_pre_basic_blocks()){
                    if(loop->find(prev) == loop->end()){
                        target_prev = prev;
                        break;
                    }
                }

                Instruction* last_instr;
                auto it = target_prev->get_instructions().end();
                it--;
                last_instr = *it;
                loop_base_bb->add_instr_begin(last_instr);
                target_prev->delete_instr(last_instr);                

                for(auto bb_in_loop : *loop)
                {
                    auto instr_list = bb_in_loop->get_instructions();

                    for(auto it = instr_list.begin(); it != instr_list.end(); it++)
                    {
                        auto instr = *it;
                        if(instr->isTerminator() || instr->is_phi() || instr->is_cmp())
                            continue;
                        else{
                            bool is_loop_inv = true;
                            for(int i = 0; i < instr->get_num_operand(); i++){
                                auto operand = instr->get_operand(i);
                                if(operand->get_name() == ""){}     //if it's const.
                                else if( loop->find(varname2bb[operand->get_name()]) != loop->end()){
                                    is_loop_inv = false;
                                }
                            }
                            if(is_loop_inv){
                                target_prev->add_instruction(instr);    
                                varname2bb[instr->get_name()] = target_prev;
                                instr->set_parent(target_prev);         
                                bb_in_loop->delete_instr(instr);
                            }   
                        }
                    }
                }
                target_prev->add_instruction(last_instr);
                loop_base_bb->delete_instr(last_instr);
            }
        }
    }
}
