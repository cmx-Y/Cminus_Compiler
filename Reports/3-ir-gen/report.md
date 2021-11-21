# Lab3 实验报告

小组成员 

（队长）姓名：陈兴焱		 学号：`PB19051157`

（队员）姓名：刘伟			 学号：`PB19051195`


## 实验难点

1. 在`void CminusfBuilder::visit(ASTVar &node)`函数中，对于当前变量的不同类型，需要进行不同的操作。这对于生成的IR代码有很大的影响。

2. 理解`visitor pattern`

   ​	在实验二时，对访问者模式有了初步的了解，但拿到实验三的实验文档时，却发现不知从何下手，通过对实验二的calculator的例子的反复研究后，终于明白了我们所要完成的任务。在逐步推进实验三的过程中，对访问者模式有了更为深入的了解。

3. c++语言困难

   ​	由于之前的课程中，主要使用的是c语言，对c++的面向对象特性了解很浅薄。在实验的过程中，通过查阅各种资料，理解了：利用虚函数机制实现多态性、类的封装与继承等知识点

## 实验设计

请写明为了顺利完成本次实验，加入了哪些亮点设计，并对这些设计进行解释。
可能的阐述方向有:

1. 如何设计全局变量
2. 遇到的难点以及解决方案
3. 如何降低生成 IR 中的冗余
4. ...

### 全局变量设计

1. 我们在`cminusf_builder.hpp`头文件的`ASTVisitor`类中，加入了两个Private变量。

```c++
Value *val;
CminusType type;
```

这两个变量分别用来存储visitor访问语法树的某个节点后得到该节点的值以及其类型，在访问者模式中，随着visitor的访问进行传递。

2. 我们使用`std::vector<Function> AllFun`来存储声明的所有函数，在函数调用时使用。
3. 我们使用`Value *cur_arg`来存储在某个函数的参数列表中当前正在存储的参数，并将其传递给`void CminusfBuilder::visit(ASTParam &node)`。
4. 我们使用`Type *cur_retTy`来存储当前函数的返回值类型，用于判断返回值是否需要类型转换。
5. 我们使用`std::map<std::string,bool> isleftvalue`数组来存储当前遇到的变量是否为左值，这将在`void CminusfBuilder::visit(ASTVar &node)`函数中起到关键作用。如果遇到的变量为左值，那么val仍然为指向当前变量地址的指针，需要进行store将具体值存储到该地址；如果遇到的变量不是左值，那么可以通过load来获取当前变量地址中存储的值，并赋给val。
6. 我们使用`bool is_retstmt`来判断某个statement是否为`retstmt`，如果不是`retstmt`，则需要进行基本块跳转，执行接下来需要的指令；如果是`retstmt`，则不需要进行基本块的跳转，并删除原先预定义的基本块（因为执行返回操作后这个基本块不会被使用，是空基本块）。后来发现`get_terminator`函数可以直接判断当前基本块的最后一个语句是或为终结语句，但是这个想法是经历了一系列思考得到的，我们在`void CminusfBuilder::visit(ASTSelectionStmt &node)`的实现中进行了保留，具有一定意义。
7. 我们使用`std::vector<bool> finalstmt`来判断某个`compound-stmt`是否是该函数中最外层的`compound-stmt`

### 难点与解决方案

1. 在`void CminusfBuilder::visit(ASTVar &node)`函数中，对于当前变量的不同类型，需要进行不同的操作。比如当前变量是否为左值，通过采用全局变量`isleftvalue`来实现；又或者对于一个指针类型变量，var为这个指针类型变量的地址，即其所指向变量类型还是指针，我们首先需要执行一次load来获取所指向的指针，再使用gep获取其地址，这在后续函数调用时遇到函数参数为指针的情况起到很大作用；对于一个数组类型的变量，我们也需要使用gep来得到数组元素的地址。注意指针类型与数组类型使用gep的区别，示例如下：

   ```c++
   if(var->get_type()->get_pointer_element_type()->is_pointer_type()){
       auto load_var = builder->create_load(var);
       var = builder->create_gep(load_var,{expr_val});
   }
   else
       var = builder->create_gep(var, {CONST_INT(0),expr_val});
   ```

2. `while`循环中，false出口有可能就是程序的末尾，这会导致空label的产生，所以我们手动添加了`ret void`语句，条件判断如下：

   ```c++
   if(builder->get_insert_block()->get_parent()->get_name()=="main" && builder->get_insert_block() == whileBB && finalstmt.size()==1){
           builder->set_insert_point(whileBB);
           if(whileBB->empty() == true)
           whileBB->add_instr_begin(builder->create_void_ret());
   }
   ```

3. `ASTCall`中，为了获取函数类型，要声明一个对象，但这个对象又会自动加入到模块的列表中，当`ASTCall`退出后，这个局部变量就会被释放，于是模块的函数链表中就会有一个指向非法区域的指针，从而发生`segmentation fault`(找这个bug找麻了/(ㄒoㄒ)/~~)

   ```c++
   auto FunTy = Function::create(AllFun[0].get_function_type(),AllFun[0].get_name(),module.get());
   ```

   solution：在`Module.cpp`中手动添加了一个删除函数

   ```c++
   void Module::pop_function()
   {
       function_list_.pop_back();
   }
   ```

4. `SelectionStmt`中，首先要考虑是否有else，其次要考虑是否有return语句，在这两种考虑下，生成label并进行跳转，最后还要考虑会不会生成空label，如果有的话要抹去

## 实验总结

此次实验有什么收获

- 对c++的面向对象特性认识更加深刻，对map、vector等容器认识更加深刻，调用类的方法的能力++
- 通过实践对上课提到的符号表的构建、标号回填技术、综合属性和继承属性的理解更加深刻

## 实验反馈 （可选 不会评分）

对本次实验的建议

无

## 组间交流 （可选）

无
