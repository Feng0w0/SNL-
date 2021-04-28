## 编译原理
本项目是基于SNL语言的编译程序，该项目拥有词法分析，语法分析（包括LL1方法和递归下降方法）,语义分析。
### 实验环境
VS2017
### 编程语言
C++
### 项目结构
snls.txt文件夹中存储的是SNL源程序。  
词法分析会读取snls.txt文件，然后生成token.txt文件,该文件包含token序列。  
语法分析会读取token.txt文件然后生成LL1tree.txt(Destree.txt)文件,该文件包含抽象树。  
语义分析会使用抽象树的根节点进行语义分析，然后生成SemanticAnalysis.txt,该文件包含符号表。  
当源程序有词法分析和语法分析错误时，程序会退出并将错误输出到error.txt文件中，语义分析错误时会直接显示在控制台窗口中。