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
    if (curIndex + step >= 0 && curIndex + step < tokNum)
        return tokList[curIndex + step];
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
    out << lexer.map.at(curTok.type) << " " << curTok.literal << endl;
}

// ＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞} ＜主函数＞
void Parser::parseProgram() {
    do {
        getNextToken();
    } while (curIndex < tokNum);
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
    if (curTok.type == TokenType::INTCON)
        out << "<无符号整数>" << endl;
}

// ＜字符串＞ ::= "｛十进制编码为32,33,35-126的ASCII字符｝"
void Parser::parseString() {
    if (curTok.type == TokenType::STRCON)
        out << "<字符串>" << endl;
}

// ＜常量说明＞ ::= const＜常量定义＞;{ const＜常量定义＞;}
void Parser::parseConstStmt() {
    do {
        getNextToken();//eat "const"
        parseConstDef();
    } while (curTok.type == TokenType::SEMICN);//如果当前token为const则说明又有一个 const<常量定义>
    out << "<常量说明>" << endl;
}

//  ＜常量定义＞ ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//               | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void Parser::parseConstDef() {
    if (curTok.type == TokenType::INTTK || curTok.type == TokenType::CHARTK) {
        do {
            getNextToken();//eat current token
        } while (curTok.type != TokenType::SEMICN);// eat token until meet ";"
    }
    out << "<常量定义>" << endl;
}

// ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void Parser::parseVarStmt() {
    do {
        parseVarDef();
//        getNextToken();
    } while (curTok.type == TokenType::SEMICN);
    out << "<变量说明>" << endl;
}

//＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )}
//＜无符号整数＞表示数组元素的个数，其值需大于0
void Parser::parseVarDef() {
    do {
        getNextToken();
    } while (curTok.type != TokenType::SEMICN);
    out << "<变量定义>" << endl;
}

// ＜声明头部＞ ::= int＜标识符＞
//              |char＜标识符＞
void Parser::parseDeclHeader() {
    getNextToken();
    has_retval.insert(pair<string, bool>(curTok.literal, true));//记录当前函数类型: 有返回值/无返回值
    getNextToken();
    out << "<声明头部>" << endl;
}

// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void Parser::parseFuncDef() {
    parseDeclHeader();
    do {
        getNextToken();
    } while (curTok.type != TokenType::RBRACE);
    getNextToken();// eat }
    out << "<有返回值函数定义>" << endl;
}

// ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void Parser::parseVoidFuncDef() {
    getNextToken();//eat void
    has_retval.insert(pair<string, bool>(curTok.literal, true));//记录当前函数类型: 有返回值/无返回值
    do {
        getNextToken();
    } while (curTok.type != TokenType::RBRACE);
    getNextToken();// eat }
    out << "<无返回值函数定义>" << endl;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Parser::parseFuncCall() {
    do {
        getNextToken();
    } while (curTok.type != TokenType::RPARENT);
    getNextToken();
    out << "<有返回值函数调用语句>" << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Parser::parseVoidFuncCall() {
    do {
        getNextToken();
    } while (curTok.type != TokenType::RPARENT);
    getNextToken();
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
    out << "<语句>" << endl;
}

//＜语句列＞ ::= ｛＜语句＞｝
void Parser::parseStmtList() {
    out << "<语句列>" << endl;
}

//＜复合语句＞ ::= ［＜常量说明＞］［＜变量说明＞］＜语句列＞
void Parser::parseMulStmt() {
    out << "<复合语句>" << endl;
}

//＜赋值语句＞ ::= ＜标识符＞＝＜表达式＞
//              |＜标识符＞'['＜表达式＞']'=＜表达式＞
void Parser::parseAssignStmt() {
    out << "<赋值语句>" << endl;
}

//＜条件语句＞ ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
void Parser::parseCondStmt() {
    out << "<条件语句>" << endl;
}

//＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞ //整型表达式之间才能进行关系运算
//          ｜＜表达式＞    //表达式为整型，其值为0条件为假，值不为0时条件为真
void Parser::parseCond() {
    out << "<条件>" << endl;
}

//＜循环语句＞ ::= while '('＜条件＞')'＜语句＞
//        | do＜语句＞while '('＜条件＞')'
//        |for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
void Parser::parseLoopStmt() {
    out << "<循环语句>" << endl;
}

//＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Parser::parseValArgList() {
    out << "<值参数表>" << endl;
}

//＜读语句＞ ::= scanf '('＜标识符＞{,＜标识符＞}')'
void Parser::parseReadStmt() {
    out << "<读语句>" << endl;
}

//＜写语句＞ ::= printf '(' ＜字符串＞,＜表达式＞ ')'
//            | printf '('＜字符串＞ ')'
//            | printf '('＜表达式＞')'
void Parser::parseWriteStmt() {
    out << "<写语句>" << endl;
}

//＜返回语句＞ ::= return['('＜表达式＞')']
void Parser::parseReturnStmt() {
    out << "<返回语句>" << endl;
}

//＜主函数＞ ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
void Parser::parseMain() {
    out << "<主函数>" << endl;
}

//＜参数表＞ ::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
//            | ＜空＞
void Parser::parseArgList() {
    out << "<参数表>" << endl;
}

//＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   //[+|-]只作用于第一个<项>
void Parser::parseExpr() {
    out << "<表达式>" << endl;
}

//＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Parser::parseItem() {
    out << "<项>" << endl;
}

//＜因子＞ ::= ＜标识符＞
//          ｜＜标识符＞'['＜表达式＞']'
//          |'('＜表达式＞')'
//          ｜＜整数＞
//          |＜字符＞
//          ｜＜有返回值函数调用语句＞
void Parser::parseFactor() {
    out << "<因子>" << endl;
}






