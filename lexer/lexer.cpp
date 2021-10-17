//
// Created by ggssh on 2021/10/16 0016.
//
#include "lexer.h"

Token &Lexer::get_token() {
    static int last_char = ' ';
    static Token token(TokenType::UNREGONIZED, "");
    // 跳过空格
    while (isspace(last_char)) {
        UPDATECHAR;
    }
    // 可能为identifier
    if (last_char == '_') {
        ident = last_char;
        while (isalnum(last_char = reader->get_char()) || last_char == '_') {
            ident += last_char;
        }
        token = Token(TokenType::IDENFR, ident);
        return token;
    }
    // 判断identifier和keyword
    if (isalpha(last_char)) {
        ident = last_char;
        while (isalnum(last_char = reader->get_char()) || last_char == '_') {
            ident += last_char;
        }
        if (ident == "const") {
            token = Token(TokenType::CONSTTK, "const");
            return token;
        } else if (ident == "int") {
            token = Token(TokenType::INTTK, "int");
            return token;
        } else if (ident == "char") {
            token = Token(TokenType::CHARTK, "char");
            return token;
        } else if (ident == "void") {
            token = Token(TokenType::VOIDTK, "void");
            return token;
        } else if (ident == "main") {
            token = Token(TokenType::MAINTK, "main");
            return token;
        } else if (ident == "if") {
            token = Token(TokenType::IFTK, "if");
            return token;
        } else if (ident == "else") {
            token = Token(TokenType::ELSETK, "else");
            return token;
        } else if (ident == "do") {
            token = Token(TokenType::DOTK, "do");
            return token;
        } else if (ident == "while") {
            token = Token(TokenType::WHILETK, "while");
            return token;
        } else if (ident == "for") {
            token = Token(TokenType::FORTK, "for");
            return token;
        } else if (ident == "scanf") {
            token = Token(TokenType::SCANFTK, "scanf");
            return token;
        } else if (ident == "printf") {
            token = Token(TokenType::PRINTFTK, "printf");
            return token;
        } else if (ident == "return") {
            token = Token(TokenType::RETURNTK, "return");
            return token;
        } else {
            token = Token(TokenType::IDENFR, ident);
            return token;
        }
    }
    // 判断数字
    if (isdigit(last_char)) {
        std::string num_str;
        do {
            num_str += (char) last_char;
            UPDATECHAR;
        } while (isdigit(last_char));
        token = Token(TokenType::INTCON, num_str);
        return token;
    }

    // 判断字符串
    if (last_char == '\"') {
        std::string str;
        while ((last_char = reader->get_char()) != '\"') {
            str += last_char;
        }
        UPDATECHAR;
        //todo 字符串中间可能会含有引号
        token = Token(TokenType::STRCON, str);
        return token;
    }

        //判断字符
    else if (last_char == '\'') {
        int count = 0;//记录被单引号包裹的字符的数量,如果大于1的话就报错
        std::string str;
        while ((last_char = reader->get_char()) != '\'') {
            str += last_char;
            count++;
        }
        if (count!=1){

        }
        // todo 如果两个单引号包裹的内容为多个字符的话要报错
        UPDATECHAR;
        token = Token(TokenType::CHARCON, str);
        return token;
    }
    //判断特殊符号
    if (last_char == '+') {
        token = Token(TokenType::PLUS, "+");
        UPDATECHAR;
        return token;
    } else if (last_char == '-') {
        token = Token(TokenType::MINU, "-");
        UPDATECHAR;
        return token;
    } else if (last_char == '*') {
        token = Token(TokenType::MULT, "*");
        UPDATECHAR;
        return token;
    } else if (last_char == '/') {
        token = Token(TokenType::DIV, "/");
        UPDATECHAR;
        return token;
    } else if (last_char == '<') {
        if (SEEK == '=') {
            reader->get_char();
            UPDATECHAR;
            token = Token(TokenType::LEQ, "<=");
            return token;
        }
        UPDATECHAR;
        token = Token(TokenType::LSS, "<");
        return token;
    } else if (last_char == '>') {
        if (SEEK == '=') {
            reader->get_char();
            UPDATECHAR;
            token = Token(TokenType::GEQ, ">=");
            return token;
        }
        UPDATECHAR;
        token = Token(TokenType::GRE, ">");
        return token;

    } else if (last_char == '=') {
        if (SEEK == '=') {
            reader->get_char();
            UPDATECHAR;
            token = Token(TokenType::EQL, "==");
            return token;
        }
        UPDATECHAR;
        token = Token(TokenType::ASSIGN, "=");
        return token;

    } else if (last_char == '!') {
        if (SEEK == '=') {
            reader->get_char();
            UPDATECHAR;
            token = Token(TokenType::NEQ, "!=");
            return token;
        } else {
            // todo 需要进行错误处理
            UPDATECHAR;
            token = Token(TokenType::UNREGONIZED, "!");
            return token;
        }
    } else if (last_char == ';') {
        UPDATECHAR;
        token = Token(TokenType::SEMICN, ";");
        return token;
    } else if (last_char == ',') {
        UPDATECHAR;
        token = Token(TokenType::COMMA, ",");
        return token;
    } else if (last_char == '(') {
        UPDATECHAR;
        token = Token(TokenType::LPARENT, "(");
        return token;
    } else if (last_char == ')') {
        UPDATECHAR;
        token = Token(TokenType::RPARENT, ")");
        return token;
    } else if (last_char == '[') {
        UPDATECHAR;
        token = Token(TokenType::LBRACK, "[");
        return token;
    } else if (last_char == ']') {
        UPDATECHAR;
        token = Token(TokenType::RBRACK, "]");
        return token;
    } else if (last_char == '{') {
        UPDATECHAR;
        token = Token(TokenType::LBRACE, "{");
        return token;
    } else if (last_char == '}') {
        UPDATECHAR;
        token = Token(TokenType::RBRACE, "}");
        return token;
    }
    // 如果遇到文件的末尾
    if (last_char == EOF) {
        token = Token(TokenType::EOFTOK, "");
        return token;
    }
    UPDATECHAR;
    token = Token(TokenType::UNREGONIZED, "");
    return token;
}

FileReader *Lexer::getReader() const {
    return reader;
}
