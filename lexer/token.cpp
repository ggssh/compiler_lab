//
// Created by ggssh on 2021/10/16 0016.
//

#include "token.h"

#include <utility>

Token::Token(TokenType type, std::string literal) : type(type), literal(std::move(literal)) {}
