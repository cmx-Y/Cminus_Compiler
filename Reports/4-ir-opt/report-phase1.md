# Lab4 实验报告-阶段一

小组成员 

姓名: 陈兴焱  学号：PB19051157

姓名: 刘伟	  学号：PB19051195

## 实验要求

阅读`Mem2Reg Pass`和`LoopSearch Pass`两部分的代码，通过自己的理解回答思考题。

## 思考题

### LoopSearch

1. **`LoopSearch`中直接用于描述一个循环的数据结构是什么？需要给出其具体类型。**

   `BBset_t`

   ```c++
   using BBset_t = std::unordered_set<BasicBlock *>;	//也就是基本块指针的集合
   ```

2. **循环入口是重要的信息，请指出`LoopSearch`中如何获取一个循环的入口？需要指出具体代码，并解释思路。**

   `LoopSearch::run()`执行流程：

   - 遍历`module`的`func_list`	
   - 对于每个`func`，通过`build_cfg()`建立`cfg`，存储在`nodes`中
   - 基于`tarjan`算法，通过`strongly_connected_components()`求出强连通片集合，存储在`sccs`(set of set)中
   - 对于每个强连通片`scc`，将其中每个`cfg`对应的`bb`插入集合`bb_set`中，就得到一个循环，通过`find_loop_base()`获取循环入口

   

   对于`find_loop_base()`**两个参数**的解释:

   - `set`：连通片中的cfg结点集合
   - `reserved`：之前删去的base集合

   

   获取循环入口的**两种办法**:

   ```c++
   CFGNodePtr LoopSearch::find_loop_base(
       CFGNodePtrSet *set,
       CFGNodePtrSet &reserved)
   {
   
       CFGNodePtr base = nullptr;						//method1
       for (auto n : *set)								//遍历set中的每个结点n
       {
           for (auto prev : n->prevs)					//遍历n的前驱集合
           {
               if (set->find(prev) == set->end())		//如果前驱集合中的某个结点不在set中		
               {
                   base = n;							//则n就是循环入口即base
               }										//因为某循环的结点集合有唯一的入口结点
           }
       }
       if (base != nullptr)
           return base;								//method2
       for (auto res : reserved)						//有可能找不到这样的结点n,所以18行不return
       {												//因为base的前驱关系可能在之前删除其他base结点时被删除了
           for (auto succ : res->succs)				//所以应该遍历reserved中的每个结点res				
           {											//再遍历res的后继集合
               if (set->find(succ) != set->end())
               {
                   base = succ;						//如果在set中找到res后继集合中的某点succ
               }										//则succ就是set的循环入口即base
           }
       }
   
       return base;
   }
   ```

   

3. **仅仅找出强连通分量并不能表达嵌套循环的结构。为了处理嵌套循环，`LoopSearch`在Tarjan algorithm的基础之上做了什么特殊处理？**

   首先解释`strongly_connected_components()`

   ```c++
   bool LoopSearch::strongly_connected_components(
       CFGNodePtrSet &nodes,
       std::unordered_set<CFGNodePtrSet *> &result)
   {
       index_count = 0;
       stack.clear();
       for (auto n : nodes)	//这个for循环是考虑到整个图有可能有多个连通片
       {						//所以需要在各个连通片中分别找强连通分量
           if (n->index == -1)
               traverse(n, result);
       }
       return result.size() != 0;
   }
   ```

   

   其次解释特殊处理：对于嵌套循环，采用的是由外向内找的方法，找到某强连通分量后，通过删除该强连通分量的base结点以破坏连通性，对得到的新结点集合进行再次查找，就可以找到内层循环（如果有的话），具体解释如下：

   ```c++
   while (strongly_connected_components(nodes, sccs))	//nodes表示cfg结点集合（可能在初始基础上删除若干点）
               {										//sccs是找到的强连通片集合
                   if (sccs.size() == 0)	break;
                   else
                   {
                       for (auto scc : sccs)			//遍历sccs
                       {
                           scc_index += 1;
                           auto base = find_loop_base(scc, reserved);	//找到该scc的循环入口
                           auto bb_set = new BBset_t;
                           std::string node_set_string = "";
                           for (auto n : *scc)			
                           {
                               bb_set->insert(n->bb);	
                               node_set_string = node_set_string + n->bb->get_name() + ',';
                           }
                           loop_set.insert(bb_set);
                           func2loop[func].insert(bb_set);
                           base2loop.insert({base->bb, bb_set});
                           loop2base.insert({bb_set, base->bb});
                           for (auto bb : *bb_set)
                           {
                               if (bb2base.find(bb) == bb2base.end())
                                   bb2base.insert({bb, base->bb});
                               else
                                   bb2base[bb] = base->bb;
                           }
                           reserved.insert(base);		//将base插入reserved集合，之后find_loop_base()会用到
                           dump_graph(*scc, func->get_name() + '_' + std::to_string(scc_index));
                           nodes.erase(base);			//将base从nodes中删去
                           for (auto su : base->succs)	//将与base关联的结点的前驱、后继关系删去
                           {							//这样就破坏了连通性，以便下一次while循环找更深层循环
                               su->prevs.erase(base);
                           }
                           for (auto prev : base->prevs)
                           {
                               prev->succs.erase(base);
                           }
   
                       } // for (auto scc : sccs)
                       for (auto scc : sccs)			//释放空间与初始化
                           delete scc;
                       sccs.clear();
                       for (auto n : nodes)
                       {
                           n->index = n->lowlink = -1;
                           n->onStack = false;
                       }
                   } // else
               }     // while (strongly_connected_components(nodes, sccs))
   ```

4. **某个基本块可以属于多层循环中，`LoopSearch`找出其所属的最内层循环的思路是什么？这里需要用到什么数据？这些数据在何时被维护？需要指出数据的引用与维护的代码，并简要分析。**

   通过`LoopSearch::get_inner_loop()`函数可以得到某基本块所属的最内层循环。

   **基本思路：**通过基本块到循环入口的映射关系找到循环入口，再通过循环入口到循环的映射关系找到所属的最内层循环

   需要用到的数据

   - `std::unordered_map<BasicBlock *, BasicBlock *> bb2base;`
   - `std::unordered_map<BasicBlock *, BBset_t *> base2loop;`

   

   数据引用代码：

   ```c++
   // LoopSearch.hpp
   // 得到bb所在最低层次的loop 
   BBset_t *get_inner_loop(BasicBlock* bb){
       if(bb2base.find(bb) == bb2base.end())
           return nullptr;					//如果bb没有对应的base，说明不在循环中，返回空指针
       return base2loop[bb2base[bb]];		//如果bb有对应的base，通过两次映射找到所属的最内层循环
   }
   ```

   

   数据维护代码：

   ```c++
   //在run()函数的 while (strongly_connected_components(nodes, sccs))中
       base2loop.insert({base->bb, bb_set});		//向base2loop插入表项，将base映射到loop
       loop2base.insert({bb_set, base->bb});
       for (auto bb : *bb_set)						//遍历循环中的每个基本块			
       {
           if (bb2base.find(bb) == bb2base.end())
               bb2base.insert({bb, base->bb});
           else
               bb2base[bb] = base->bb;				//向bb2base插入表项，将bb映射到该loop的base
        }
   //由于每次while循环都要维护这些数据，所以这些数据对应的就是基本块与其所在的最内层循环的关系
   ```

### Mem2reg

1. **支配性**：如果从流图起点开始，每条到达n的路径都要经过d，则称d支配n，写成d dom n。特别地，每个结点支配自身。

   **严格支配性**：若d是n的支配结点且d$\ne	$n，则称d严格支配n。

   **直接支配性**：在n的严格支配结点集合中，与n最接近的结点称为n的直接支配结点，记作IDom(n).

   **支配边界**：满足（1）n支配m的一个前驱；（2）n不严格支配m	这两个条件的结点m的集合称为n的支配边界，记作DF(n)。

2. **$\phi$节点的概念**：当x具有多个前驱节点时，$\phi$节点用于唯一确定x的最终值。

   **引入$\phi$节点的理由**：在SSA中，一个变量只能赋值一次，而这个变量在IR中会被多次定义，使用$\phi$节点可以唯一确定该变量在当前节点的值。

3. 在没有使用Mem2Reg Pass前，对一个变量的多次赋值（以左值的方式出现）需要多次创建空间，并将变量的值store到分配的空间中，需要使用该变量时（该变量以右值的方式出现时），使用load指令取出存储的值。分析如下：

   ```c++
   define i32 @main() {
   label_entry:
     %op0 = alloca i32				//为变量a分配地址
     store i32 0, i32* %op0		//a = 0
     %op1 = add i32 1, 2
     store i32 %op1, i32* %op0		//a = 1 + 2
     %op2 = load i32, i32* %op0	//获取a = a * 4中以右值方式出现的a的值
     %op3 = mul i32 %op2, 4
     store i32 %op3, i32* %op0		//存储a = a * 4中以左值方式出现的a的值
     ret i32 0
   }
   ```

4. ①当变量以全局变量或者以GEP指针类型变量方式出现时，与原IR代码相同，store与load指令没有改变。如下：

   ```c++
   @globVar = global i32 zeroinitializer
   store i32 1, i32* @globVar
   %op8 = load i32, i32* @globVar
   ```

   ```c++
   %op5 = getelementptr [10 x i32], [10 x i32]* %op0, i32 0, i32 5
   store i32 999, i32* %op5
   ```

   ②如果变量在当前基本块的前驱中没有被定义过，即在数据流中第一次以左值形式出现，此时store指令没有变化，因为$\phi$函数不能从前驱基本块中确定当前变量的值。

   这一点在给出的示例代码中似乎没有得到很好的体现，因为源代码中b=2333是符合该类型的，但是在使用了常量传播和死代码删除后，示例IR代码中直接使用常量2333代替了b，如下：

   ```c++
   %op7 = call i32 @func(i32 2333)
   ```

   ③如果变量在当前基本块的前驱中被定义过，表示变量在数据流中已经出现过且分配了空间，若变量以右值形式出现，此时指令发生变化，通过phi指令确定当前变量来自于哪个基本块的数据流后，可以直接使用该变量的值，而不需要使用load指令重新获取该变量的值。因为当前基本块中的活跃变量必定来源与前驱中已经定义过的变量，值可以直接使用。如下：

   ```c++
     %op9 = phi i32 [ %arg0, %label_entry ], [ 0, %label6 ]
     ret i32 %op9
   ```

5. **放置phi节点的代码：**在`Mem2Reg::generate_phi()`函数中，放置phi节点。

   **使用支配树的信息：**

   `Mem2Reg Pass`首先构建了一个支配树，并在`Mem2Reg::generate_phi()`函数中，确定支配树的所有支配边界，即遍历`dominators_->get_dominance_frontier(bb)`，并为每一个还没有phi指令的`bb_dominance_frontier_bb`插入phi指令。如下：

   ```c++
   for ( auto bb_dominance_frontier_bb : dominators_->get_dominance_frontier(bb))
               {
                   if ( bb_has_var_phi.find({bb_dominance_frontier_bb, var}) == bb_has_var_phi.end() )
                   { 
                       // generate phi for bb_dominance_frontier_bb & add bb_dominance_frontier_bb to work list
                       auto phi = PhiInst::create_phi(var->get_type()->get_pointer_element_type(), bb_dominance_frontier_bb);
                       phi->set_lval(var);
                       bb_dominance_frontier_bb->add_instr_begin( phi );
                       work_list.push_back( bb_dominance_frontier_bb );
                       bb_has_var_phi[{bb_dominance_frontier_bb, var}] = true;
                   }
               }
   
   ```

   在`Mem2Reg::re_name(BasicBlock *bb)`中，需要对后继基本块依次rename，这时候需要使用支配树的信息，即遍历`dominators_->get_dom_tree_succ_blocks(bb)`。如下：

   ```c++
   for ( auto dom_succ_bb : dominators_->get_dom_tree_succ_blocks(bb) )
   {
       re_name(dom_succ_bb);
   }
   ```

   

### 代码阅读总结

学会了找图强连通分量的 `tarjan` 算法，读懂LoopSearch pass的关键就在于理解这个算法，以及如何处理循环嵌套。

这次的优化代码有一些内容课堂上还没有学习过，比如对phi节点的定义等，读起来有一些吃力。但是阅读完成后，确实对代码优化由了初步的认识，有所收获！

### 实验反馈 （可选 不会评分）

对本次实验的建议

### 组间交流 （可选）

本次实验和哪些组（记录组长学号）交流了哪一部分信息
