//
// Created by ggssh on 2021/10/20 0020.
//

#include "parser.h"

static Token curTok = Token(TokenType::UNREGONIZED, "");// 当前token
Token unregonized = Token(TokenType::UNREGONIZED, "");

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

// ＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
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

// ＜无符号整数＞ ::= ＜非零数字＞｛＜数字＞｝| 0
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
        getNextToken();//eat ";"
    } while (curTok.type == TokenType::CONSTTK);//如果当前token为const则说明又有一个 const<常量定义>
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
    out << "<变量说明>" << endl;
}

//＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )}
//＜无符号整数＞表示数组元素的个数，其值需大于0
void Parser::parseVarDef() {
    out << "<变量定义>" << endl;
}

// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void Parser::parseFuncDef() {
    out << "<有返回值函数定义>" << endl;
}

// ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void Parser::parseVoidFuncDef() {
    out << "<无返回值函数定义>" << endl;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Parser::parseFuncCall() {
    out << "<有返回值函数调用语句>" << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Parser::parseVoidFuncCall() {
    out << "<无返回值函数调用语句>" << endl;
}

//＜步长＞ ::= ＜无符号整数＞
void Parser::parseStep() {
    parseUnsignedInteger();
    out << "<步长>" << endl;
}




