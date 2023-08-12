#pragma once 
#include "system/core/lib.h"
#include "commandHandler.h"
#include "program.h"

// Token types
enum TokenType { COMMAND, LPAREN, RPAREN, COMMA, NUMBER, IDENTIFIER, IF, ELSE, END, COMMENT, EOL, END_OF_SCRIPT };

// Token structure
struct Token {
    TokenType type;
    String value;
};


class Interpreter {
public:
    Interpreter();
    Program processScript(const String& script);

private:
    // Lexer functions
    Token nextToken();
    Token parseNumber();
    Token parseIdentifier();
    void skipWhitespace();

    // Parser functions
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<CommandStatement> parseCommandStatement();
    std::shared_ptr<IfStatement> parseIfStatement();

    //Syntax error
    String currentPos();
    void displaySyntaxError(const String& commandName);

    // Lexer state
    String input;
    size_t pos;
    Token currentToken;
};
