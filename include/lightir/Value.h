#ifndef SYSYC_VALUE_H
#define SYSYC_VALUE_H

#include <string>
#include <list>
#include <iostream>

class Type;
class Value;

//eg1.   %op7 = add i32 %op5, %op6
//  %op5.use_list: size 1 
//                 use_val_name op7
//                 use_no       0
//  %op6.use_list: size 1
//                 use_val_name op7
//                 use_no       1

//eg2.  %op7 = add i32 2, 3
//      %op10 = add i32 %op7, 4
//first %op7'use_list: size 1
//                     use_val_name op10
//                     use_no       0
//second op7'use_list: size 1
//                     use_val_name op10
//                     use_no       0

struct Use
{
    Value *val_;
    unsigned arg_no_;     // the no. of operand, e.g., func(a, b), a is 0, b is 1
    Use(Value *val, unsigned no) : val_(val), arg_no_(no) {}
};

class Value
{
public:
    explicit Value(Type *ty, const std::string &name = "");
    ~Value() = default;

    Type *get_type() const { return type_; }

    std::list<Use> &get_use_list() { return use_list_; }

    void add_use(Value *val, unsigned arg_no = 0);

    bool set_name(std::string name) { 
        if (name_ == "")
        {
            name_=name;
            return true;
        }   
        return false; 
    }
    std::string get_name() const;

    void replace_all_use_with(Value *new_val);
    void remove_use(Value *val);

    virtual std::string print() = 0;
private:
    Type *type_;
    std::list<Use> use_list_;   // (which instr)who use this value
    std::string name_;    // should we put name field here ?
};

#endif // SYSYC_VALUE_H
