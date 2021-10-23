//
// Created by ggssh on 2021/10/20 0020.
//

#include "parser.h"

static Token curTok = Token(TokenType::UNREGONIZED, "");// 当前token
Token unregonized = Token(TokenType::UNREGONIZED, "");
unordered_map<string, bool> has_retval;

Parser::Parser(Lexer &lexer, ofstream &out) : lexer(lexer), out(out) {
    while (true) {
        Token token = lexer.get_token();
        if (token.type == TokenType::EOFTOK) break;
        tokList.emplace_back(token);
    }
    tokNum = tokList.size();
    curIndex = 0;//将当前的读到的token的位置index
}

Token &Parser::seekN(int step) {
    if (curIndex + step >= 0 && curIndex + step - 1 < tokNum)
        return tokList[curIndex + step - 1];
    else {
        unregonized = Token(TokenType::UNREGONIZED, "");
        return unregonized;
    }
}

void Parser::getNextToken() {
    if (curIndex < tokNum) {
        curTok = tokList[curIndex++];
    } else {
        curTok = unregonized;
    }
//    if (curTok.type != TokenType::EOFTOK)
    // 在更新curTok的同时向文件输出流中写入token的信息
    out << curTok.get_type() << " " << curTok.literal << endl;
//    out << lexer.map.at(curTok.type) << " " << curTok.literal << endl;
}

//note 所有的函数只处理到所属于当前表达式的最后一个token 不需要使用getNextToken

// ＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞} ＜主函数＞
void Parser::parseProgram() {
    getNextToken();//获得第一个token,此时tokIndex=1;
    //判断是否存在常量说明
    if (curTok.type == TokenType::CONSTTK) {
        parseConstStmt();
        getNextToken();
    }
    //判断是否存在变量说明
    if ((curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK)
        && (seekN(2).type == TokenType::SEMICN || seekN(2).type == TokenType::COMMA ||
            seekN(2).type == TokenType::LBRACK)) {
        parseVarStmt();
        getNextToken();
    }
    //判断是否存在函数定义
    while (((curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) &&
            seekN(2).type == TokenType::LPARENT && seekN(1).type != TokenType::MAINTK)
           || (curTok.type == TokenType::VOIDTK && seekN(2).type == TokenType::LPARENT &&
               seekN(1).type != TokenType::MAINTK)) {
        if (((curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) &&
             seekN(2).type == TokenType::LPARENT)) {
            parseFuncDef();
            getNextToken();//eat 有返回值函数定义
        } else if (curTok.type == TokenType::VOIDTK && seekN(2).type == TokenType::LPARENT) {
            parseVoidFuncDef();
            getNextToken();//eat 无返回值函数定义
        }
    }
    parseMain();//分析主函数
    out << "<程序>" << endl;
}

// ＜整数＞ ::= ［＋｜－］＜无符号整数＞
void Parser::parseInteger() {
    if (curTok.type == TokenType::PLUS || curTok.type == TokenType::MINU) {
        getNextToken();//eat +\-
        parseUnsignedInteger();
    } else if (curTok.type == TokenType::INTCON) {
        parseUnsignedInteger();
    }
    out << "<整数>" << endl;
}

// ＜无符号整数＞ ::= ＜非零数字＞｛＜数字＞｝
//                | 0
void Parser::parseUnsignedInteger() {
    if (curTok.type == TokenType::INTCON) {
        out << "<无符号整数>" << endl;
    }
}

// ＜字符串＞ ::= "｛十进制编码为32,33,35-126的ASCII字符｝"
void Parser::parseString() {
    if (curTok.type == TokenType::STRCON) {
        out << "<字符串>" << endl;
    }
}

// ＜常量说明＞ ::= const＜常量定义＞;{ const＜常量定义＞;}
void Parser::parseConstStmt() {
    getNextToken();//eat const
    parseConstDef();
    getNextToken();//当前token为 ;
    while (seekN(1).type == TokenType::CONSTTK) {
        getNextToken();
        getNextToken();//eat "const"
        parseConstDef();
        getNextToken();//当前的token为;
    }
    out << "<常量说明>" << endl;
}

//  ＜常量定义＞ ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//               | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void Parser::parseConstDef() {
//    int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
    if (curTok.type == TokenType::INTTK) {
        getNextToken();//eat int
        getNextToken();//eat 标识符
        getNextToken();//eat =
        parseInteger();
        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();
            getNextToken();// ,
            getNextToken();//eat 标识符
            getNextToken();//eat =
            parseInteger();
        }
    }
//    char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
    else if (curTok.type == TokenType::CHARTK) {
        getNextToken();//eat char
        getNextToken();//eat 标识符
        getNextToken();//eat =
        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();//eat 字符
            getNextToken();//eat ,
            getNextToken();//eat 标识符
            getNextToken();//eat =
        }
    }
    out << "<常量定义>" << endl;
}

// ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void Parser::parseVarStmt() {
    parseVarDef();
    getNextToken();//当前token为 ;
    while ((seekN(1).type == TokenType::INTTK || seekN(1).type == TokenType::CHARTK) &&
           seekN(3).type != TokenType::LPARENT) { ;//如果下一个token属于类型标识符{
        getNextToken();//eat ;
        parseVarDef();
        getNextToken();//当前token为 ;
    }
    out << "<变量说明>" << endl;
}

//＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )}
//＜无符号整数＞表示数组元素的个数，其值需大于0
void Parser::parseVarDef() {
    getNextToken();//eat 类型标识符
    if (seekN(1).type == TokenType::LBRACK) {
        getNextToken();//eat 标识符
        getNextToken();//eat [
        parseUnsignedInteger();//处理无符号整数
        getNextToken();//eat 无符号整数
    }
    while (seekN(1).type == TokenType::COMMA) {
        getNextToken();
        getNextToken();//eat ,
        if (curTok.type == TokenType::LBRACK) {
            getNextToken();//eat [
            parseUnsignedInteger();//处理无符号整数
            getNextToken();//eat 无符号整数
        }
    }
//    do {
//        getNextToken();//eat ,
//        getNextToken();//eat 标识符
//        if (curTok.type == TokenType::LBRACK) {
//            getNextToken();//eat [
//            parseUnsignedInteger();//处理无符号整数
//            getNextToken();//当前token为 ]
//        }
//    } while (seekN(1).type == TokenType::SEMICN);
    out << "<变量定义>" << endl;
}

// ＜声明头部＞ ::= int＜标识符＞
//              |char＜标识符＞
void Parser::parseDeclHeader() {
    getNextToken();//eat int/char
    has_retval.insert(pair<string, bool>(curTok.literal, true));//记录当前函数类型: 有返回值
    out << "<声明头部>" << endl;
}

// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void Parser::parseFuncDef() {
    parseDeclHeader();
    getNextToken();//eat 声明头部
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseArgList();
        getNextToken();//eat 参数表
    }else{
        out << "<参数表>" << endl;
        getNextToken();//eat (
    }
//    getNextToken();//eat (
//    parseArgList();
//    getNextToken();//eat 参数表
    getNextToken();//eat )
    getNextToken();//eat {
    parseMulStmt();
    getNextToken();//eat 复合语句
    out << "<有返回值函数定义>" << endl;
}

// ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void Parser::parseVoidFuncDef() {
    getNextToken();//eat void
    has_retval.insert(pair<string, bool>(curTok.literal, false));//记录当前函数类型: 无返回值
    getNextToken();//eat 标识符
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseArgList();
        getNextToken();//eat 参数表
    }else{
        out << "<参数表>" << endl;
        getNextToken();//eat (
    }
    getNextToken();//eat )
    getNextToken();//eat {
    parseMulStmt();
    getNextToken();//eat 复合语句
    out << "<无返回值函数定义>" << endl;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Parser::parseFuncCall() {
    getNextToken();//eat 标识符
//    getNextToken();//eat (
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseValArgList();
        getNextToken();//eat 值参数表
    }else{
        out<<"<值参数表>"<<endl;
        getNextToken();//eat (
    }
    out << "<有返回值函数调用语句>" << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Parser::parseVoidFuncCall() {
    getNextToken();//eat 标识符
//    getNextToken();//eat (
    if (seekN(1).type!=TokenType::RPARENT){
        getNextToken();//eat (
        parseValArgList();
        getNextToken();//eat 值参数表
    }else{
        out<<"<值参数表>"<<endl;
        getNextToken();//eat (
    }
    out << "<无返回值函数调用语句>" << endl;
}

//＜步长＞ ::= ＜无符号整数＞
void Parser::parseStep() {
    parseUnsignedInteger();
    out << "<步长>" << endl;
}

//＜语句＞ ::= ＜条件语句＞
//          ｜＜循环语句＞
//          | '{'＜语句列＞'}'
//          | ＜有返回值函数调用语句＞;
//          |＜无返回值函数调用语句＞;
//          ｜＜赋值语句＞;
//          ｜＜读语句＞;
//          ｜＜写语句＞;
//          ｜＜空＞;
//          |＜返回语句＞;
void Parser::parseStmt() {
    if (curTok.type == TokenType::IFTK) {
        parseCondStmt();
    } else if (curTok.type == TokenType::WHILETK || curTok.type == TokenType::DOTK || curTok.type == TokenType::FORTK) {
        parseLoopStmt();
        //评测机文法规则有问题
        if (seekN(1).type == TokenType::SEMICN){
            out<<"<语句>"<<endl;
            getNextToken();//eat )
        }
    } else if (curTok.type == TokenType::LBRACE) {
        getNextToken();//eat {
        parseStmtList();
        getNextToken();//eat 语句列
    } else if (curTok.type == TokenType::IDENFR) {
        //有返回值函数
        if (has_retval[curTok.literal]) {
            parseFuncCall();
            getNextToken();//eat 有返回值函数调用语句
        } else if (seekN(1).type == TokenType::ASSIGN || seekN(1).type == TokenType::LBRACK) {
            parseAssignStmt();
            getNextToken();//eat 赋值语句
        } else if (!has_retval[curTok.literal]) {
            parseVoidFuncCall();
            getNextToken();//eat 无返回值函数调用语句
        }
    } else if (curTok.type == TokenType::SCANFTK) {
        parseReadStmt();
        getNextToken();//eat 读语句
    } else if (curTok.type == TokenType::PRINTFTK) {
        parseWriteStmt();
        getNextToken();//eat 写语句
    } else if (curTok.type == TokenType::RETURNTK) {
        parseReturnStmt();
        getNextToken();//eat 返回语句
    }
//    else {
//        out << "<空>" << endl;
//    }
    out << "<语句>" << endl;
}

//＜语句列＞ ::= ｛＜语句＞｝
void Parser::parseStmtList() {
    if (curTok.type != TokenType::RBRACE) {
        while (seekN(1).type != TokenType::RBRACE&&curIndex<tokNum) {
            parseStmt();
            if (seekN(1).type != TokenType::RBRACE&&curIndex<tokNum) {
                getNextToken();
            }
        }
    }
    out << "<语句列>" << endl;
}

//＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void Parser::parseMulStmt() {
    if (curTok.type == TokenType::CONSTTK) {
        parseConstStmt();
//        ［＜常量说明＞］［＜变量说明＞］＜语句列＞
        if (seekN(1).type == TokenType::INTTK || seekN(1).type == TokenType::CHARTK) {
            getNextToken();//eat 常量说明
            parseVarStmt();
        }
        getNextToken();//eat 变量说明/常量说明
        parseStmtList();
    }
        // ［＜变量说明＞］＜语句列＞
    else if (curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) {
        parseVarStmt();
        getNextToken();//eat 变量说明
        parseStmtList();
    }
        // ＜语句列＞
    else {
        parseStmtList();
    }
    out << "<复合语句>" << endl;
}

//＜赋值语句＞ ::= ＜标识符＞＝＜表达式＞
//              |＜标识符＞'['＜表达式＞']'=＜表达式＞
void Parser::parseAssignStmt() {
    getNextToken();//eat 标识符
    //＜标识符＞＝＜表达式＞
    if (curTok.type == TokenType::ASSIGN) {
        getNextToken();//eat =
        parseExpr();
    }
        //＜标识符＞'['＜表达式＞']'=＜表达式＞
    else {
        getNextToken();//eat [
        parseExpr();
        getNextToken();//eat 表达式
        getNextToken();//eat ]
        getNextToken();//eat =
        parseExpr();
    }
    out << "<赋值语句>" << endl;
}

//＜条件语句＞ ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
void Parser::parseCondStmt() {
    getNextToken();//eat if
    getNextToken();//eat (
    parseCond();
    getNextToken();//eat 条件
    getNextToken();// )
    parseStmt();
    if (seekN(1).type == TokenType::ELSETK) {
        getNextToken();//eat 语句
        getNextToken();//eat else
        parseStmt();
    }
    out << "<条件语句>" << endl;
}

//＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞ //整型表达式之间才能进行关系运算
//          ｜＜表达式＞    //表达式为整型，其值为0条件为假，值不为0时条件为真
void Parser::parseCond() {
    parseExpr();
    if (seekN(1).type == TokenType::LSS
        || seekN(1).type == TokenType::LEQ
        || seekN(1).type == TokenType::GRE
        || seekN(1).type == TokenType::GEQ
        || seekN(1).type == TokenType::EQL
        || seekN(1).type == TokenType::NEQ) {
        getNextToken();//eat 表达式
        getNextToken();//eat 关系运算符
        parseExpr();
    }
    out << "<条件>" << endl;
}

//＜循环语句＞ ::= while '('＜条件＞')'＜语句＞
//        | do＜语句＞while '('＜条件＞')'
//        |for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
void Parser::parseLoopStmt() {
//    while '('＜条件＞')'＜语句＞
    if (curTok.type == TokenType::WHILETK) {
        getNextToken();//eat while
        getNextToken();//eat (
        parseCond();
        getNextToken();//eat 条件
        getNextToken();//eat )
        parseStmt();
    }
//    do＜语句＞while '('＜条件＞')'
    else if (curTok.type == TokenType::DOTK) {
        getNextToken();//eat do
        parseStmt();
        getNextToken();//eat 语句
        getNextToken();//eat while
        getNextToken();//eat (
        parseCond();
        getNextToken();//eat 条件
    }
//    for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
    else if (curTok.type == TokenType::FORTK) {
        getNextToken();//eat for
        getNextToken();//eat (
        getNextToken();//eat 标识符
        getNextToken();//eat =
        parseExpr();
        getNextToken();//eat 表达式
        getNextToken();//eat ;
        parseCond();
        getNextToken();//eat 条件
        getNextToken();//eat ;
        getNextToken();//eat 标识符
        getNextToken();//eat =
        getNextToken();//eat 标识符
        getNextToken();//eat +/-
        parseStep();
        getNextToken();//eat 步长
        getNextToken();//eat )
        parseStmt();
    }
    out << "<循环语句>" << endl;
}

//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Parser::parseValArgList() {
    // 如果不为空
    if (curTok.type != TokenType::RPARENT) {
        parseExpr();
        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();//eat 表达式
            getNextToken();//eat ,
            parseExpr();
        }
    }
    out << "<值参数表>" << endl;
}

//＜读语句＞ ::= scanf '('＜标识符＞{,＜标识符＞}')'
void Parser::parseReadStmt() {
    getNextToken();//eat scanf
    getNextToken();//eat (
    while (seekN(1).type == TokenType::COMMA) {
        getNextToken();//eat 标识符
        getNextToken();//eat ,
    }
    getNextToken();//eat 标识符
    out << "<读语句>" << endl;
}

//＜写语句＞ ::= printf '(' ＜字符串＞,＜表达式＞ ')'
//            | printf '('＜字符串＞ ')'
//            | printf '('＜表达式＞')'
void Parser::parseWriteStmt() {
    getNextToken();//eat printf
    getNextToken();//eat (
    if (curTok.type == TokenType::STRCON) {
        //printf '(' ＜字符串＞,＜表达式＞ ')'
        if (seekN(1).type == TokenType::COMMA) {
            parseString();
            getNextToken();//eat 字符串
            getNextToken();//eat ,
            parseExpr();
            getNextToken();//eat 表达式
        }
            // printf '('＜字符串＞ ')'
        else {
            parseString();
            getNextToken();//eat 字符串
        }
    }
        // printf '('＜表达式＞')'
    else {
        parseExpr();
        getNextToken();//eat 表达式
    }
    out << "<写语句>" << endl;
}

//＜返回语句＞ ::= return['('＜表达式＞')']
void Parser::parseReturnStmt() {
    //如果返回不为空
    if (seekN(1).type == TokenType::LPARENT) {
        getNextToken();// eat return
        getNextToken();// eat (
        parseExpr();
        getNextToken();//eat 表达式
    }
    out << "<返回语句>" << endl;
}

//＜主函数＞ ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
void Parser::parseMain() {
    getNextToken();//eat void
    getNextToken();//eat main
    getNextToken();//eat (
    getNextToken();//eat )
    getNextToken();//eat {
    parseMulStmt();//处理复合语句
    getNextToken();//eat 复合语句
    out << "<主函数>" << endl;
}

//＜参数表＞ ::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
//            | ＜空＞
void Parser::parseArgList() {
    //＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
    if (curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) {
        getNextToken();//eat 类型标识符
        while (seekN(1).type == TokenType::COMMA) {
            getNextToken();//eat 标识符
            getNextToken();//eat ,
            getNextToken();//eat 类型标识符
        }
    }
//    else {
//        out << "<空>" << endl;
//    }
    out << "<参数表>" << endl;
}

//＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   //[+|-]只作用于第一个<项>
void Parser::parseExpr() {
    if (curTok.type == TokenType::PLUS || curTok.type == TokenType::MINU) {
        getNextToken(); //eat +/-
    }
    parseItem();
    while (seekN(1).type == TokenType::PLUS || seekN(1).type == TokenType::MINU) {
        getNextToken();//eat 项
        getNextToken();//eat 加法运算符
        parseItem();
    }
    out << "<表达式>" << endl;
}

//＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Parser::parseItem() {
    parseFactor();
    while (seekN(1).type == TokenType::MULT || seekN(1).type == TokenType::DIV) {
        getNextToken();//eat 因子
        getNextToken();//eat 乘法运算符
        parseFactor();
    }
    out << "<项>" << endl;
}

//＜因子＞ ::= ＜标识符＞
//          ｜＜标识符＞'['＜表达式＞']'
//          |'('＜表达式＞')'
//          ｜＜整数＞
//          |＜字符＞
//          ｜＜有返回值函数调用语句＞
void Parser::parseFactor() {
    if (curTok.type == TokenType::IDENFR) {
        if (has_retval[curTok.literal]) {
            parseFuncCall();//有返回值函数调用语句
        } else if (seekN(1).type == TokenType::LBRACK) {
            getNextToken();//eat 标识符
            getNextToken();//eat [
            parseExpr();
            getNextToken();//eat 表达式
        }
        //否则就是个单纯的标识符
    } else if (curTok.type == TokenType::LPARENT) {
        getNextToken();//eat (
        parseExpr();
        getNextToken();//eat 表达式
    } else if (curTok.type == TokenType::INTCON || curTok.type == TokenType::PLUS || curTok.type == TokenType::MINU) {
        parseInteger();
    } else if (curTok.type == TokenType::CHARCON) {
        //do nothing
    }
    out << "<因子>" << endl;
}