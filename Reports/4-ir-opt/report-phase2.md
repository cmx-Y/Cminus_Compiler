# Lab4 实验报告

小组成员 姓名 学号

陈兴焱	PB19051157

刘伟		PB19051195

## 实验要求

请按照自己的理解，写明本次实验需要干什么

​	在助教已实现的`mem2reg`、`LoopSearch`两个pass的基础上，实现新的三个pass，以达到对IR代码的三个优化：常量传播与死代码删除、循环不变式外提、活跃变量分析

## 实验难点

实验中遇到哪些挑战

- 对助教实现的两个pass的进一步理解：
  - 如何找到循环并找到对应的`loop_base`
  - 在`mem2reg` pass之后，实现了`SSA`，要如何运用`SSA` 相关的特性
- 常量传播与死代码删除
  - 对常量指令的直接删除导致的`Segmentation fault`。
  - 维护了一个全局`map<instruction, constant>`而非基本块内部的`map<instruction, constant>`导致了一系列分析困难，如全局变量的多次赋值，同名op等。
  - 需要考虑的指令类型较多，容易遗漏。
- 循环不变式外提：
  - 由于一开始未调用`func->set_instr_name()`导致了数据依赖关系难以进行分析
  - 对于指令的不当删除导致了`Segmentation fault`
- 活跃变量分析
  - `phi`指令中所用到的变量都是`phi`指令所在模块的活跃变量，需要补全。
  - 需要考虑的指令类型过多，容易遗漏。

## 实验设计

* 常量传播
  实现思路：对于不同的指令，如果其操作数都是常量且指令得到的结果也是常量，那么可以计算得到的常量替换该指令，并将之后所有对该指令的引用替换成这个常量。

  相应代码：

  类似助教给出的整形值的常数折叠实现，在头文件中定义处理其他指令的函数，如下：

  ```c++
  	ConstantInt *compute(
          Instruction::OpID op,
          ConstantInt *value1,
          ConstantInt *value2);
      ConstantFP *fcompute(
          Instruction::OpID op,
          ConstantFP *value1,
          ConstantFP *value2
      );
      ConstantInt *fticompute(
          Instruction::OpID op,
          ConstantFP *value1
      );
      ConstantFP *itfcompute(
          Instruction::OpID op,
          ConstantInt *value1
      );
      ConstantInt *zextcompute(
          Instruction::OpID op,
          ConstantInt *value1
      );
      ConstantInt *cmpcompute(
          CmpInst::CmpOp op,
          ConstantInt *value1,
          ConstantInt *value2
      );
      ConstantInt *fcmpcompute(
          FCmpInst::CmpOp op,
          ConstantFP *value1,
          ConstantFP *value2
      );
  ```

  每个函数对应了一种类型的指令，用于计算当指令的操作数都是常数时，指令得到结果的常数值。

  在调用上述函数之前，需要先判断指令的操作数是否全都是常数，定义了一个标志is_const_value，当所有操作数都为常数是才为真。代码如下：

  ```
  for(auto i = 0; i < ins->get_num_operand() && is_const_value == true; i++){
      auto operand = ins->get_operand(i);
      if(cast_constantfp(operand) == nullptr && cast_constantint(operand) == nullptr){
          is_const_value = false;
      }
  }
  ```

  此外，还需要考虑branch、store、load指令。如果分支指令为条件跳转，那么判断这个条件是否为常数，如果为常数，则可以直接得到跳转的地址，不需要进行判断。同时，如果load指令的目标为当前模块中被store的变量，那么可以直接使用这个变量的常数值替代，不需要store与load。代码如下所示：

  ```c++
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
  ```

  在上述过程中，每一条要删除的指令都会被存在一个指令数组`instodelete`中，在访问完所有模块以后，同一进行删除。如下：

  ```c++
  for(auto ins: instodelete){
  	bb->delete_instr(ins);
  }
  ```

  优化前后的IR对比（举一个例子）并辅以简单说明：

  以testcase-3为例。

  调用Pass前，label_entry：

  ```c++
  label_entry:
    %op0 = mul i32 0, 1
    %op1 = mul i32 %op0, 2
    %op2 = mul i32 %op1, 3
    %op3 = mul i32 %op2, 4
    %op4 = mul i32 %op3, 5
    %op5 = mul i32 %op4, 6
    %op6 = mul i32 %op5, 7
    store i32 %op6, i32* @opa
    %op7 = mul i32 1, 2
    %op8 = mul i32 %op7, 3
    %op9 = mul i32 %op8, 4
    %op10 = mul i32 %op9, 5
    %op11 = mul i32 %op10, 6
    %op12 = mul i32 %op11, 7
    %op13 = mul i32 %op12, 8
    store i32 %op13, i32* @opb
    %op14 = mul i32 2, 3
    %op15 = mul i32 %op14, 4
    %op16 = mul i32 %op15, 5
    %op17 = mul i32 %op16, 6
    %op18 = mul i32 %op17, 7
    %op19 = mul i32 %op18, 8
    %op20 = mul i32 %op19, 9
    store i32 %op20, i32* @opc
    %op21 = mul i32 3, 4
    %op22 = mul i32 %op21, 5
    %op23 = mul i32 %op22, 6
    %op24 = mul i32 %op23, 7
    %op25 = mul i32 %op24, 8
    %op26 = mul i32 %op25, 9
    %op27 = mul i32 %op26, 10
    store i32 %op27, i32* @opd
    %op28 = load i32, i32* @opa
    %op29 = load i32, i32* @opb
    %op30 = icmp slt i32 %op28, %op29
    %op31 = zext i1 %op30 to i32
    %op32 = icmp ne i32 %op31, 0
    br i1 %op32, label %label33, label %label39
  ```

  调用该Pass后，label_entry:

  ```c++
  label_entry:
    store i32 0, i32* @opa
    store i32 40320, i32* @opb
    store i32 362880, i32* @opc
    store i32 1814400, i32* @opd
    br label %label33
  ```

  可以看到，所有mul指令全都被删除，因为其操作数全都是常量，可以直接计算得出。并且，将计算结果存储在一个变量地址中的store指令以及从地址中读取数据的load指令也被删除，因为可以直接用常量去代替变量，不需要重新赋值。在br指令中，原先的条件跳转变成了直接跳转，因为判断条件的结果可以直接被确定。


* 循环不变式外提
  实现思路：对于一个循环，遍历每个基本块的每条指令，对于每条指令的每个操作数，如果是常量，或者不是在该循环中被定值，那么该指令就是循环不变式，可以外提。
  相应代码：

  ```c++
  for(auto bb: func->get_basic_blocks())
                  for(auto instr : bb->get_instructions())
                          varname2bb[instr->get_name()] = instr->get_parent();
  ```

  以上代码，遍历所有指令，确定每一个左值是在哪一个基本块中被定义，结果存储在`std::map<std::string, BasicBlock *> varname2bb`中

  

  ```c++
  for(int i = 0; i < instr->get_num_operand(); i++){
                                  auto operand = instr->get_operand(i);
                                  if(operand->get_name() == ""){}     //if it's const.
                                  else if( loop->find(varname2bb[operand->get_name()]) != loop->end()){
                                      is_loop_inv = false;
                                  }
                              }
  ```

  以上代码，遍历某指令的所有操作数，如果某操作数在该循环中被定值，那么该指令不是循环不变式

  

  ```c++
  if(is_loop_inv){
                                  target_prev->add_instruction(instr);    
                                  varname2bb[instr->get_name()] = target_prev;
                                  instr->set_parent(target_prev);         
                                  bb_in_loop->delete_instr(instr);
                              }
  ```

  以上代码，如果某指令是循环不变式，将该指令外提到目标基本块，并且将定值关系更新

  

  ```c++
  				Instruction* last_instr;
                  auto it = target_prev->get_instructions().end();
                  it--;
                  last_instr = *it;
                  loop_base_bb->add_instr_begin(last_instr);
                  target_prev->delete_instr(last_instr);
  ```

                  target_prev->add_instruction(last_instr);
                  loop_base_bb->delete_instr(last_instr);

  以上代码，对目标基本块的终止指令进行处理，以便有外提的循环不变式加入后，目标基本块的终止指令仍然时目标基本块的最后一条指令



优化前后的IR对比（举一个例子）并辅以简单说明：

优化前：

    ; ModuleID = 'cminus'
    source_filename = "../tests/4-ir-opt/testcases/LoopInvHoist/testcase-1.cminus"
    
    declare i32 @input()
    
    declare void @output(i32)
    
    declare void @outputFloat(float)
    
    declare void @neg_idx_except()
    
    define void @main() {
    label_entry:
      br label %label3
    label3:                                                ; preds = %label_entry, %label58
      %op61 = phi i32 [ %op64, %label58 ], [ undef, %label_entry ]
      %op62 = phi i32 [ 1, %label_entry ], [ %op60, %label58 ]
      %op63 = phi i32 [ %op65, %label58 ], [ undef, %label_entry ]
      %op5 = icmp slt i32 %op62, 10000
      %op6 = zext i1 %op5 to i32
      %op7 = icmp ne i32 %op6, 0
      br i1 %op7, label %label8, label %label9
    label8:                                                ; preds = %label3
      br label %label11
    label9:                                                ; preds = %label3
      call void @output(i32 %op61)
      ret void
    label11:                                                ; preds = %label8, %label16
      %op64 = phi i32 [ %op61, %label8 ], [ %op55, %label16 ]
      %op65 = phi i32 [ 0, %label8 ], [ %op57, %label16 ]
      %op13 = icmp slt i32 %op65, 10000
      %op14 = zext i1 %op13 to i32
      %op15 = icmp ne i32 %op14, 0
      br i1 %op15, label %label16, label %label58
    label16:                                                ; preds = %label11
      %op19 = mul i32 %op62, %op62
      %op21 = mul i32 %op19, %op62
      %op23 = mul i32 %op21, %op62
      %op25 = mul i32 %op23, %op62
      %op27 = mul i32 %op25, %op62
      %op29 = mul i32 %op27, %op62
      %op31 = mul i32 %op29, %op62
      %op33 = mul i32 %op31, %op62
      %op35 = mul i32 %op33, %op62
      %op37 = sdiv i32 %op35, %op62
      %op39 = sdiv i32 %op37, %op62
      %op41 = sdiv i32 %op39, %op62
      %op43 = sdiv i32 %op41, %op62
      %op45 = sdiv i32 %op43, %op62
      %op47 = sdiv i32 %op45, %op62
      %op49 = sdiv i32 %op47, %op62
      %op51 = sdiv i32 %op49, %op62
      %op53 = sdiv i32 %op51, %op62
      %op55 = sdiv i32 %op53, %op62
      %op57 = add i32 %op65, 1
      br label %label11
    label58:                                                ; preds = %label11
      %op60 = add i32 %op62, 1
      br label %label3
    }

优化后：

```cpp
; ModuleID = 'cminus'
source_filename = "../tests/4-ir-opt/testcases/LoopInvHoist/testcase-1.cminus"

declare i32 @input()

declare void @output(i32)

declare void @outputFloat(float)

declare void @neg_idx_except()

define void @main() {
label_entry:
  br label %label3
label3:                                                ; preds = %label_entry, %label58
  %op61 = phi i32 [ %op64, %label58 ], [ undef, %label_entry ]
  %op62 = phi i32 [ 1, %label_entry ], [ %op60, %label58 ]
  %op63 = phi i32 [ %op65, %label58 ], [ undef, %label_entry ]
  %op5 = icmp slt i32 %op62, 10000
  %op6 = zext i1 %op5 to i32
  %op7 = icmp ne i32 %op6, 0
  br i1 %op7, label %label8, label %label9
label8:                                                ; preds = %label3
  %op19 = mul i32 %op62, %op62
  %op21 = mul i32 %op19, %op62
  %op23 = mul i32 %op21, %op62
  %op25 = mul i32 %op23, %op62
  %op27 = mul i32 %op25, %op62
  %op29 = mul i32 %op27, %op62
  %op31 = mul i32 %op29, %op62
  %op33 = mul i32 %op31, %op62
  %op35 = mul i32 %op33, %op62
  %op37 = sdiv i32 %op35, %op62
  %op39 = sdiv i32 %op37, %op62
  %op41 = sdiv i32 %op39, %op62
  %op43 = sdiv i32 %op41, %op62
  %op45 = sdiv i32 %op43, %op62
  %op47 = sdiv i32 %op45, %op62
  %op49 = sdiv i32 %op47, %op62
  %op51 = sdiv i32 %op49, %op62
  %op53 = sdiv i32 %op51, %op62
  %op55 = sdiv i32 %op53, %op62
  br label %label11
label9:                                                ; preds = %label3
  call void @output(i32 %op61)
  ret void
label11:                                                ; preds = %label8, %label16
  %op64 = phi i32 [ %op61, %label8 ], [ %op55, %label16 ]
  %op65 = phi i32 [ 0, %label8 ], [ %op57, %label16 ]
  %op13 = icmp slt i32 %op65, 10000
  %op14 = zext i1 %op13 to i32
  %op15 = icmp ne i32 %op14, 0
  br i1 %op15, label %label16, label %label58
label16:                                                ; preds = %label11
  %op57 = add i32 %op65, 1
  br label %label11
label58:                                                ; preds = %label11
  %op60 = add i32 %op62, 1
  br label %label3
}
```

在label16之后的很多操作，都被前提到了label8处

* 活跃变量分析
  实现思路：

  * 计算每个模块的`use`和`def`集合，其中`use`集合表示：所有引用前未在该模块中被定值的变量；`def`集合表示：所有定值前未在该模块中被引用的变量，当遇到`phi`指令时，需要计算对应的`phi_uses[S,B]`,其中S是B的后继。
  * 利用`use`和`def`集合计算每个模块的`IN`和`OUT`集合，分别表示该模块入口和出口处的活跃变量，计算公式为：

    * $OUT[B] = \cup_{S是B的后继}IN[S]\cup_{S是B的后继}phi\_uses[S,B]$
    * $IN[B]=use_B\cup(OUT[B]-def_B)$

​		相应的代码：

​		维护的use，def，IN，OUT，以及phi_uses集合的定义如下：

```c++
std::map<BasicBlock* , std::set<std::string>> def, use, IN, OUT;
std::map<BasicBlock* , std::map<BasicBlock* , std::set<std::string>>> phi_uses;
```

​		计算use和def集合时，要对模块中的每条指令进行考虑，当指令定值或引用的变量满足定义时，将其加入对应的集合，以整数和浮点数的基本运算为例：

```c++
if(ins->is_add()||ins->is_fadd()||ins->is_sub()||ins->is_fsub()||ins->is_mul()||ins->is_fmul()||ins->is_div()||ins->is_fdiv()||ins->is_cmp()||ins->is_fcmp()||ins->is_zext()||ins->is_gep()){
    for(auto operand : ins->get_operands()){                        
        if(cur_def.find(operand->get_name()) == cur_def.end() && operand->get_name() != "") //no define before use
            cur_use.insert(operand->get_name());
            }
        if(cur_use.find(ins->get_name()) == cur_use.end())              					//no use before define
            cur_def.insert(ins->get_name());    
}
```

​		其中cur_def和cur_use为定义的临时set类型变量，用于存储一个模块中所有的def和use的变量，在完成对一个模块中所有指令的访问以后，将加入到def和use这两个map中，如下：

```c++
def.insert({bb, cur_def});
use.insert({bb, cur_use});
```

​		其余指令类型根据各自的操作数以及运算方式有类似的判断是否为def或use变量的方式，在此不一一赘述。

​		计算phi_uses集合时，如果遇到的指令类型为phi指令，则不需要将操作数加入use，而是将其加入当前模块与其对应前驱模块的phi_uses集合中，如下：

```c++
else if(ins->is_phi()){
    if(cur_use.find(ins->get_name()) == cur_use.end())
        cur_def.insert(ins->get_name());
    for(auto i = 0; i < ins->get_num_operand(); i += 2){
        if(ins->get_operand(i)->get_name() != ""){
            phi_uses[bb][dynamic_cast<BasicBlock*>(ins->get_operand(i + 1))].insert(ins->get_operand(i)->get_name());
        }
    }
}
```

​		当所有模块全部遍历完成后，便得到了所有模块的use和def变量集合以及模块之间对应的phi_uses集合，可以开始对各个模块的活跃变量进行计算。这里采取循环方式计算，用flag作为继续循环的标志，如果在当前循环中，IN或OUT的内容发生了改变，则flag=true，表示继续进行下一次循环，直到IN和OUT集合不发生改变为止，代码如下：

```c++
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
```

​		由于计算时，没有将phi指令的操作数加入当前模块的use集合中，最后得到的模块的入口处活跃变量IN[B]会缺少这部分变量，因此选择在计算完成后，将其补全，代码如下：

```c++
for(auto bb : func_->get_basic_blocks()){
    for(auto pre : bb->get_pre_basic_blocks()){
        for(auto var: phi_uses[bb][pre]){
            if(IN[bb].find(var) == IN[bb].end()){
                IN[bb].insert(var);
            }
        }
    }
}
```

​		至此，已完成所有活跃变量的计算，之后调用print函数打印即可**（这里由于个人代码实现方式的与助教不同，因此对print函数进行了些许修改使其可以按要求打印所有的活跃变量）**。

### 实验总结

此次实验有什么收获

- 进一步掌握了cpp
- 理解了优化pass的基本思想
- 了解了常量传播与死代码删除、循环不变式外提、活跃变量分析三种优化的思想

### 实验反馈 （可选 不会评分）

对本次实验的建议

### 组间交流 （可选）

本次实验和哪些组（记录组长学号）交流了哪一部分信息
