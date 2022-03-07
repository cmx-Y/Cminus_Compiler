## 项目简介 

 	该项目借鉴于llvm，基于cmake工具进行构建编译，用c++实现了一个轻量级编译器，代码变化的主要流程：cminus源码->llvm中间代码->risc-v

## 项目组成

### 前端模块

​	分析输入的cminus文件，通过词法分析器和文法分析器构建对应于该cminus程序的抽象语法树

- 基于Flex提取cminus源码中的token，其中应用了**正则表达式**
  - `/src/parser/lexical_analyzer.l`
- 基于Bison对token构成的句子进行分析，并根据给定的cminus文法，形成语法树
  - `/src/parser/syntax_analyzer.y`

### 中间模块

​	根据前端模块提供的语法树构建抽象语法树，然后遍历抽象语法树，生成llvmir，然后进行中间代码优化

- 将语法树转化为抽象语法树
  - `/src/common/ast.cpp`
- 通过**访问者模式**对抽象语法树遍历，生成BasicBlock
  - `/src/cminusfc/my_cminusf_builder.cpp`
- 采用循环不变式外提等策略对llvmir进行优化
  - `/src/optimization/LoopInvHoist.cpp ` 等

### 后端模块

  根据BasicBlock中的指令信息，建立DAG图，然后对DAG图进行遍历，实现`risc-v`代码的输出

​	`/src/backend`

