//
// Created by ggssh on 2021/10/16 0016.
//

#include "token.h"

void Token::display() {
    std::cout<<type<<" "<<literal<<std::endl;
}

Token::Token(TokenType type, const std::string &literal) : type(type), literal(literal) {}
