//
// Created by ggssh on 2021/10/20 0020.
//

#ifndef COMPILER_LAB_PARSER_H
#define COMPILER_LAB_PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include "lexer.h"

using namespace std;

// todo 全局符号表记录函数的属性
class Parser {
private:


    Lexer &lexer;
    std::ofstream &out;

    vector<Token> tokList;
    unsigned int tokNum;
    unsigned int curIndex;//当前token下标

    /**
     * 获得下一个token,并输出其值
     */
    void getNextToken();

    /**
     * 往后看几个token
     * @param index
     * @return
     */
    Token &seekN(int index);

public:
    Parser(Lexer &lexer, ofstream &out);

    /**
     * 程序
     */
    void parseProgram();

    /**
     * 无符号整数
     */
    void parseUnsignedInteger();

    /**
     * 整数
     */
    void parseInteger();

    /**
     * 字符串
     */
    void parseString();

    /**
     * 常量说明
     */

    void parseConstStmt();

    /**
     * 常量定义
     */
    void parseConstDef();

    /**
     * 变量说明
     */
    void parseVarStmt();

    /**
     * 变量定义
     */
    void parseVarDef();

    /**
     * 有返回值函数定义
     */
    void parseFuncDef();

    /**
     * 无返回值函数定义
     */
    void parseVoidFuncDef();

    /**
     * 有返回值函数调用语句
     */
    void parseFuncCall();

    /**
     * 无返回值函数调用语句
     */
    void parseVoidFuncCall();

    /**
     * 步长
     */
    void parseStep();
};

#endif //COMPILER_LAB_PARSER_H
