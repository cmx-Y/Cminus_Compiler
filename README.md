##项目简介
  该项目借鉴于llvm，用c++实现了一个轻量级编译器，代码变化的主要流程：cminus源码->llvm中间代码->risc-v。
##项目组成
###前端模块
- 基于Flex提取cminus源码中的token，其中应用了**正则表达式**
- 基于Bison对token构成的句子进行分析，并根据给定的cminus文法，形成抽象语法树
###中间模块
