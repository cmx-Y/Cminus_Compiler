#include "cminusf_builder.hpp"
#include "PassManager.hpp"
#include "LoopSearch.hpp"
#include "Dominators.h"
#include "Mem2Reg.hpp"
#include "LoopSearch.hpp"
#include "LoopInvHoist.hpp"
#include "ActiveVars.hpp"
#include "ConstPropagation.hpp"
#include "SelDAGNodes.hpp"
#include "AsmPrinter.hpp"
#include <iostream>
#include <fstream>
#include <memory>

using namespace std::literals::string_literals;

void print_help(std::string exe_name) {
    std::cout << "Usage: " << exe_name <<
        " [ -h | --help ] [ -o <target-file> ] [ -emit-llvm ] [-mem2reg] [-loop-search] [-loop-inv-hoist] [-const-propagation] [-active-vars] <input-file>" << std::endl;
}

int main(int argc, char **argv) {
    std::string target_path;
    std::string input_path;

    bool mem2reg = false;
    bool const_propagation = false;
    bool activevars = false;
    bool loop_inv_hoist = false;
    bool loop_search = false;
    bool emit = false;

    for (int i = 1;i < argc;++i) {
        if (argv[i] == "-h"s || argv[i] == "--help"s) {
            print_help(argv[0]);
            return 0;
        } else if (argv[i] == "-o"s) {
            if (target_path.empty() && i + 1 < argc) {
                target_path = argv[i + 1];
                i += 1;
            } else {
                print_help(argv[0]);
                return 0;
            }
        } else if (argv[i] == "-emit-llvm"s) {
            emit = true;
        } else if (argv[i] == "-mem2reg"s) {
            mem2reg = true;
        } else if (argv[i] == "-loop-search"s) {
            loop_search = true;
        } else if (argv[i] == "-loop-inv-hoist"s) {
            loop_inv_hoist = true;
        } else if (argv[i] == "-const-propagation"s) {
            const_propagation = true;
        } else if (argv[i] == "-active-vars"s) {
            activevars = true;
        } else {
            if (input_path.empty()) {
                input_path = argv[i];
            } else {
                print_help(argv[0]);
                return 0;
            }
        }
    }
    if (input_path.empty()) {
        print_help(argv[0]);
        return 0;
    }

    if (target_path.empty()) {
        auto pos = input_path.rfind('.');
        if (pos == std::string::npos) {
            std::cerr << argv[0] << ": input file " << input_path << " has unknown filetype!" << std::endl;
            return -1;
        } else {
            if (input_path.substr(pos) != ".cminus") {
                std::cerr << argv[0] << ": input file " << input_path << " has unknown filetype!" << std::endl;
                return -1;
            }
            if (emit) {
                target_path = input_path.substr(0, pos);
            } else {
                target_path = input_path.substr(0, pos);
            }
        }
    }

    auto s = parse(input_path.c_str());
    auto a = AST(s);
    CminusfBuilder builder;
    //ASTPrinter printer;
    //printer.visit(*a.get_root());
    a.run_visitor(builder);    

    auto m = builder.getModule();

    m->set_print_name();
    PassManager PM(m.get());

    if( mem2reg )
    {
        PM.add_pass<Mem2Reg>();
    }
    if( loop_search )
    {
        PM.add_pass<LoopSearch>();
    }
    if( const_propagation )
    {
        PM.add_pass<ConstPropagation>(true);
    }
    if( activevars )
    {
        PM.add_pass<ActiveVars>();
    }
    if( loop_inv_hoist )
    {
        PM.add_pass<LoopInvHoist>(true);
    }
    PM.run();

    std::string ASM;
    ASM += "\tjal main\n";
    SelDAGBuilder sel_dag_builder(m.get());
    sel_dag_builder.run();
    AsmPrinter asm_printer;
    for(int i = 0; i < sel_dag_builder.get_root_num(); i++){
        ASM += asm_printer.visit( *((RootSDNode*)(sel_dag_builder.get_root(i))) );
    }
    std::cout << ASM;
    
    std::ofstream asm_file;
    std::string asm_fname = target_path+".s";
    asm_file.open(asm_fname, std::ios::out);
    asm_file << ASM;
    asm_file.close();

    auto IR = m->print();

    std::ofstream output_stream;
    auto output_file = target_path+".ll";
    output_stream.open(output_file, std::ios::out);
    output_stream << "; ModuleID = 'cminus'\n";
    output_stream << "source_filename = \""+ input_path +"\"\n\n";
    output_stream << IR;
    output_stream.close();
    if (!emit) {
        auto command_string = "clang -O0 -w "s + target_path + ".ll -o " + target_path + " -L. -lcminus_io";
        int re_code0 = std::system(command_string.c_str());
        /*command_string = "rm "s + target_path + ".ll";
        int re_code1 = std::system(command_string.c_str());
        if(re_code0==0 && re_code1==0) return 0;
        else return 1;*/
    }

    return 0;
}
