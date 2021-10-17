//
// Created by ggssh on 2021/10/16 0016.
//
#include <iostream>

#ifndef COMPILER_LAB_TOKEN_H
#define COMPILER_LAB_TOKEN_H

enum TokenType {
    EOFTOK = 0,// 文件结束
    IDENFR,// 标识符
    INTCON,// 整数
    CHARCON,// 字符
    STRCON,// 字符串
    CONSTTK,// const
    INTTK,// int
    CHARTK,// char
    VOIDTK,// void
    MAINTK,// main
    IFTK,// if
    ELSETK,// else
    DOTK,// do
    WHILETK,// while
    FORTK,// for
    SCANFTK,// scanf
    PRINTFTK,// printf
    RETURNTK,// return
    PLUS,// +
    MINU,// -
    MULT,// *
    DIV,// /
    LSS,// <
    LEQ,// <=
    GRE,// >
    GEQ,// >=
    EQL,// ==
    NEQ,// !=
    ASSIGN,// =
    SEMICN,// ;
    COMMA,// ,
    LPARENT,// (
    RPARENT,// )
    LBRACK,// [
    RBRACK,// ]
    LBRACE,// {
    RBRACE,  // }
    UNREGONIZED //无法被识别
};


class Token {
public:
    Token(TokenType type, const std::string &literal);
// token的属性类型
    std::string literal; // token的值
    TokenType type;
    std::string get_type();
private:
    // 用于进行测试输出
    void display();

};

#endif //COMPILER_LAB_TOKEN_H
