//
// Created by ggssh on 2021/10/16 0016.
//
#include <iostream>
#include <unordered_map>
#include "token.h"
#include "file_reader.h"

using namespace std;

#define UPDATECHAR (last_char = reader->get_char())
#define SEEK (reader->seek())

#ifndef COMPILER_LAB_LEXER_H
#define COMPILER_LAB_LEXER_H

class Lexer {
private:
    std::string ident;
    FileReader *reader;

public:
    /**
     * 记录enum与其对应的string
     */
    unordered_map<TokenType, std::string> map;

    /**
     *
     * @param reader
     */
    explicit Lexer(FileReader *reader) : reader(reader) {
        map.insert(pair<TokenType, std::string>(TokenType::IDENFR, "IDENFR"));
        map.insert(pair<TokenType, std::string>(TokenType::INTCON, "INTCON"));
        map.insert(pair<TokenType, std::string>(TokenType::CHARCON, "CHARCON"));
        map.insert(pair<TokenType, std::string>(TokenType::STRCON, "STRCON"));
        map.insert(pair<TokenType, std::string>(TokenType::CONSTTK, "CONSTTK"));
        map.insert(pair<TokenType, std::string>(TokenType::INTTK, "INTTK"));
        map.insert(pair<TokenType, std::string>(TokenType::CHARTK, "CHARTK"));
        map.insert(pair<TokenType, std::string>(TokenType::VOIDTK, "VOIDTK"));
        map.insert(pair<TokenType, std::string>(TokenType::MAINTK, "MAINTK"));
        map.insert(pair<TokenType, std::string>(TokenType::IFTK, "IFTK"));
        map.insert(pair<TokenType, std::string>(TokenType::ELSETK, "ELSETK"));
        map.insert(pair<TokenType, std::string>(TokenType::DOTK, "DOTK"));
        map.insert(pair<TokenType, std::string>(TokenType::WHILETK, "WHILETK"));
        map.insert(pair<TokenType, std::string>(TokenType::FORTK, "FORTK"));
        map.insert(pair<TokenType, std::string>(TokenType::SCANFTK, "SCANFTK"));
        map.insert(pair<TokenType, std::string>(TokenType::PRINTFTK, "PRINTFTK"));
        map.insert(pair<TokenType, std::string>(TokenType::RETURNTK, "RETURNTK"));
        map.insert(pair<TokenType, std::string>(TokenType::PLUS, "PLUS"));
        map.insert(pair<TokenType, std::string>(TokenType::MINU, "MINU"));
        map.insert(pair<TokenType, std::string>(TokenType::MULT, "MULT"));
        map.insert(pair<TokenType, std::string>(TokenType::DIV, "DIV"));
        map.insert(pair<TokenType, std::string>(TokenType::LSS, "LSS"));
        map.insert(pair<TokenType, std::string>(TokenType::LEQ, "LEQ"));
        map.insert(pair<TokenType, std::string>(TokenType::GRE, "GRE"));
        map.insert(pair<TokenType, std::string>(TokenType::GEQ, "GEQ"));
        map.insert(pair<TokenType, std::string>(TokenType::EQL, "EQL"));
        map.insert(pair<TokenType, std::string>(TokenType::NEQ, "NEQ"));
        map.insert(pair<TokenType, std::string>(TokenType::ASSIGN, "ASSIGN"));
        map.insert(pair<TokenType, std::string>(TokenType::SEMICN, "SEMICN"));
        map.insert(pair<TokenType, std::string>(TokenType::COMMA, "COMMA"));
        map.insert(pair<TokenType, std::string>(TokenType::LPARENT, "LPARENT"));
        map.insert(pair<TokenType, std::string>(TokenType::RPARENT, "RPARENT"));
        map.insert(pair<TokenType, std::string>(TokenType::LBRACK, "LBRACK"));
        map.insert(pair<TokenType, std::string>(TokenType::RBRACK, "RBRACK"));
        map.insert(pair<TokenType, std::string>(TokenType::LBRACE, "LBRACE"));
        map.insert(pair<TokenType, std::string>(TokenType::RBRACE, "RBRACE"));
    }

    Token &get_token();
};

#endif //COMPILER_LAB_LEXER_H
