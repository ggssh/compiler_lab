## 编译原理实验
***
### 实验要求
#### 词法分析
* 输入形式:文件中的源代码
* 输出形式:token种别名称 whitespace token值
  如:
    ~~~
    CONSTTK const
    INTTK int
    IDENFR const1
    ASSIGN =
    INTCON 1
    COMMA ,
    IDENFR const2
    ASSIGN =
    MINU -
    INTCON 100
    SEMICN ;
    CONSTTK const
    CHARTK char
    ~~~
#### 语法分析
* 输入形式:文件中的源代码
* 输出形式:
  * 按词法分析识别单词的顺序，按行输出每个单词的信息（要求同词法分析作业，对于预读的情况不能输出）
  * 在下列高亮显示的语法分析成分分析结束前，另起一行输出当前语法成分的名字，形如“<常量说明>”（注：未要求输出的语法成分仍需要分析）
    如:
    ~~~
    CONSTTK const
    INTTK int
    IDENFR const1
    ASSIGN =
    INTCON 1
    <无符号整数>
    <整数>
    COMMA ,
    IDENFR const2
    ASSIGN =
    MINU -
    INTCON 100
    <无符号整数>
    <整数>
    <常量定义>
    SEMICN ;
    CONSTTK const
    CHARTK char
    IDENFR const3
    ASSIGN =
    CHARCON _
    <常量定义>
    ~~~
***
### 开发环境
* Windows:MinGW w64 6.0
* Ubuntu20.04 : gcc 9.3.0
* Cmake >= 3.16
***
### 目前已实现功能
* 从文件中读取源代码经分析后将结果输出到文件中(词法分析)
* 将词法分析得到的token返回给语法分析器进行递归下降分析
* ERROR和WARNING输出的接口
***
### 总结
总体来说难度不大,递归下降分析算是语法分析中最简单且最直接的方法了。但是需要处理的细节很多,很考验debug的耐心,很折磨,写代码之前一定要认真思考。