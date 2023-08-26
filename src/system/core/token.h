#pragma once
#include "system/core/lib.h"

// Token types
enum TokenType
{
    COMMAND,
    VARIABLE,
    NUMBER,
    IDENTIFIER,
    COMMENT,
    IF,
    ELSE,
    FOR,
    WHILE,
    BLOCK,
    END,
    VAR,
    ASSIGN,
    ADD, INCREMENT, DECREMENT,
    LITERAL,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    AND,
    OR,
    EQUAL,
    NOT_EQUAL,
    NOT,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS,
    GREATER,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    DOT,
    COMMA,
    VECTOR2,
    VECTOR3,
    EOL,
    END_OF_SCRIPT,
    END_OF_EXPRESSION,
    NONE
};


// Token structure
struct Token
{
    TokenType type = NONE;
    String value = "";

    static inline String tokenTypetoString(TokenType t)
    {
        switch (t)
        {
        case COMMAND:
            return "COMMAND";
        case VARIABLE:
            return "VARIABLE";
        case NUMBER:
            return "NUMBER";
        case IDENTIFIER:
            return "IDENTIFIER";
        case COMMENT:
            return "COMMENT";
        case IF:
            return "IF";
        case ELSE:
            return "ELSE";
        case FOR:
            return "FOR";
        case WHILE:
            return "WHILE";
        case BLOCK:
            return "BLOCK";
        case END:
            return "END";
        case VAR:
            return "VAR";
        case ASSIGN:
            return "ASSIGN";
        case ADD:
            return "ADD";
        case LITERAL:
            return "LITERAL";
        case SUBTRACT:
            return "SUBTRACT";
        case MULTIPLY:
            return "MULTIPLY";
        case DIVIDE:
            return "DIVIDE";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case EQUAL:
            return "EQUAL";
        case NOT_EQUAL:
            return "NOT_EQUAL";
        case NOT:
            return "NOT";
        case LESS_EQUAL:
            return "LESS_EQUAL";
        case GREATER_EQUAL:
            return "GREATER_EQUAL";
        case LESS:
            return "LESS";
        case GREATER:
            return "GREATER";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        case LBRACKET:
            return "LBRACKET";
        case RBRACKET:
            return "RBRACKET";
        case DOT:
            return "DOT";
        case COMMA:
            return "COMMA";
        case VECTOR2:
            return "VECTOR2";
        case VECTOR3:
            return "VECTOR3";
        case EOL:
            return "EOL";
        case END_OF_SCRIPT:
            return "END_OF_SCRIPT";
        case END_OF_EXPRESSION:
            return "END_OF_EXPRESSION";
        case NONE:
            return "NONE";
        default:
            return "UNKNOWN";
        }
    }


    String toString()
    {
        switch (type){
        case COMMAND:
            return "COMMAND(" + value + ")";
        case VARIABLE:
            return "VARIABLE(" + value + ")";
        case NUMBER:
            return "NUMBER(" + value + ")";
        case IDENTIFIER:
            return "IDENTIFIER(" + value + ")";
        case COMMENT:
            return "COMMENT(" + value + ")";
        case LITERAL:
            return "LITERAL(" + value + ")";
        case VECTOR2:
            return "VECTOR2(" + value + ")";
        case VECTOR3:
            return "VECTOR3(" + value + ")";
        default:
            return tokenTypetoString(type);
        }
    }
};
