#include "ActiveVars.hpp"

std::map<BasicBlock* , std::set<std::string>> def, use, IN, OUT;
std::map<BasicBlock* , std::map<BasicBlock* , std::set<std::string>>> phi_uses;

void ActiveVars::run()
{
    std::ofstream output_active_vars;
    output_active_vars.open("active_vars.json", std::ios::out);
    output_active_vars << "[";
    for (auto &func : this->m_->get_functions()) {
        if (func->get_basic_blocks().empty()) {
            continue;
        }
        else
        {
            func_ = func;  

            func_->set_instr_name();
            live_in.clear();
            live_out.clear();
            
            // 在此分析 func_ 的每个bb块的活跃变量，并存储在 live_in live_out 结构内
            for(auto bb : func_->get_basic_blocks()){
                std::set<std::string> cur_def = {}, cur_use = {};
                for(auto ins : bb->get_instructions()){
                    /*if(ins->get_name()!=""){
                        varname2bb.insert({ins->get_name(), ins->get_parent()});
                    }*/
                    if(ins->is_add()||ins->is_fadd()||ins->is_sub()||ins->is_fsub()||ins->is_mul()||ins->is_fmul()||ins->is_div()||ins->is_fdiv()||ins->is_cmp()||ins->is_fcmp()||ins->is_zext()||ins->is_gep()){
                        for(auto operand : ins->get_operands()){                        //no define before use
                            if(cur_def.find(operand->get_name()) == cur_def.end() && operand->get_name() != "")
                                cur_use.insert(operand->get_name());
                        }
                        if(cur_use.find(ins->get_name()) == cur_use.end())              //no use before define
                            cur_def.insert(ins->get_name());    
                    }
                    else if(ins->is_phi()){
                        if(cur_use.find(ins->get_name()) == cur_use.end())
                            cur_def.insert(ins->get_name());
                        for(auto i = 0; i < ins->get_num_operand(); i += 2){
                            if(ins->get_operand(i)->get_name() != ""){
                                phi_uses[bb][dynamic_cast<BasicBlock*>(ins->get_operand(i + 1))].insert(ins->get_operand(i)->get_name());
                            }
                        }
                    }
                    else if(ins->is_call()){
                        for(auto i = 1; i < ins->get_num_operand(); i++){
                            if(cur_def.find(ins->get_operand(i)->get_name()) == cur_def.end() && ins->get_operand(i)->get_name() != "")
                                cur_use.insert(ins->get_operand(i)->get_name());
                        }
                        if(!ins->is_void()){
                            if(cur_use.find(ins->get_name()) == cur_use.end())
                                cur_def.insert(ins->get_name());
                        }
                    }
                    else if(ins->is_alloca()){
                        if(cur_use.find(ins->get_name()) == cur_use.end()){
                            cur_def.insert(ins->get_name());
                        }
                    }
                    else if(ins->is_store()){
                        for(auto operand : ins->get_operands()){
                            if(cur_def.find(operand->get_name()) == cur_def.end() && operand->get_name() != "")
                                cur_use.insert(operand->get_name());
                        }
                    }
                    else if(ins->is_load()){
                        for(auto operand : ins->get_operands()){
                            if(cur_def.find(operand->get_name()) == cur_def.end() && operand->get_name() != "")
                                cur_use.insert(operand->get_name());
                        }
                        if(cur_use.find(ins->get_name()) == cur_use.end())
                            cur_def.insert(ins->get_name());
                    }
                    else if(ins->is_ret()){
                        if(ins->get_num_operand() <= 0)
                            continue;
                        else{
                            for(auto operand : ins->get_operands()){
                                if(cur_def.find(operand->get_name()) == cur_def.end() && operand->get_name() != "")
                                    cur_use.insert(operand->get_name());
                            }
                        }
                    }
                    else if(ins->is_br()){
                        if(ins->get_num_operand() <= 1){
                            continue;
                        }
                        else{
                            if(cur_def.find(ins->get_operand(0)->get_name()) == cur_def.end() && ins->get_operand(0)->get_name() != "")
                                cur_use.insert(ins->get_operand(0)->get_name());
                        }
                    }
                }

                def.insert({bb, cur_def});
                use.insert({bb, cur_use});
                if(bb->get_terminator()->is_ret())
                    OUT[bb] = {};
            }
        }
    }
    auto flag = true;                       //check if IN and OUT not change
    while(flag){
        flag = false;
        for (auto &func : this->m_->get_functions()) {
            if (func->get_basic_blocks().empty()) {
                continue;
            }
            else
            {
                func_ = func;  

                func_->set_instr_name();

                for(auto bb : func_->get_basic_blocks()){
                    for(auto succ : bb->get_succ_basic_blocks()){
                        for(auto var : IN[succ]){
                            if(OUT[bb].find(var) == OUT[bb].end()){
                                OUT[bb].insert(var);
                                flag = true;
                            }
                        }
                        for(auto var : phi_uses[succ][bb]){
                            if(OUT[bb].find(var) == OUT[bb].end()){
                                OUT[bb].insert(var);
                                flag = true;
                            }
                        }
                    }
                    for(auto var : use[bb]){
                        if(IN[bb].find(var) == IN[bb].end()){
                            IN[bb].insert(var);
                            flag = true;
                        }
                    }
                    for(auto var : OUT[bb]){
                        if(def[bb].find(var) == def[bb].end() && IN[bb].find(var) == IN[bb].end()){
                            IN[bb].insert(var);
                            flag = true;
                        }
                    }
                } 

            }
        }
    }
    
    for (auto func : this->m_->get_functions()) {
        if (func->get_basic_blocks().empty()) {
            continue;
        }
        func_ = func;
        func_->set_instr_name();
        for(auto bb : func_->get_basic_blocks()){
            for(auto pre : bb->get_pre_basic_blocks()){
                for(auto var: phi_uses[bb][pre]){
                    if(IN[bb].find(var) == IN[bb].end()){
                        IN[bb].insert(var);
                        flag = true;
                    }
                }
            }
        }
        live_in.clear();
        live_out.clear();
        live_in = IN;
        live_out = OUT;
        output_active_vars << print();
        output_active_vars << ",";
    }
    output_active_vars << "]";
    output_active_vars.close();
    return ;
}

std::string ActiveVars::print()
{
    std::string active_vars;
    active_vars +=  "{\n";
    active_vars +=  "\"function\": \"";
    active_vars +=  func_->get_name();
    active_vars +=  "\",\n";

    active_vars +=  "\"live_in\": {\n";
    for (auto bb : func_->get_basic_blocks()) {
        
        if (live_in[bb].size() == 0) {
                continue;
        } else {
            
            active_vars +=  "  \"";
            active_vars +=  bb->get_name();
            active_vars +=  "\": [" ;
            for(auto p: live_in[bb]){
                active_vars +=  "\"%";
                active_vars +=  p;
                active_vars +=  "\",";
            }
            active_vars += "]" ;
            active_vars += ",\n";   
            
        }
    }
    active_vars += "\n";
    active_vars +=  "    },\n";
    
    active_vars +=  "\"live_out\": {\n";
    for (auto bb : func_->get_basic_blocks()) {
        
        if (live_out[bb].size() == 0) {
                continue;
        } 
        else {
            
                active_vars +=  "  \"";
                active_vars +=  bb->get_name();
                active_vars +=  "\": [" ;
            for(auto p: live_out[bb]){
                
                active_vars +=  "\"%";
                active_vars +=  p;
                active_vars +=  "\",";
            }
                active_vars += "]";
                active_vars += ",\n";  
        }
    }
    active_vars += "\n";
    active_vars += "    }\n";

    active_vars += "}\n";
    active_vars += "\n";
    return active_vars;
}