#pragma once
#include "system/core/lib.h"

// Token types
enum TokenType { 
    COMMAND, VARIABLE, NUMBER, IDENTIFIER, COMMENT,
    IF, ELSE, END,
    ADD, LITERAL, SUBTRACT, MULTIPLY, DIVIDE, 
    AND, OR, EQUAL, NOT_EQUAL, NOT, LESS_EQUAL, GREATER_EQUAL, LESS, GREATER, 
    LPAREN, RPAREN, COMMA, 
    EOL, END_OF_SCRIPT, END_OF_EXPRESSION 
};

// Token structure
struct Token {
    TokenType type;
    String value;
};

