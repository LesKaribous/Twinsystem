#pragma once 
#include "system/core/lib.h"
#include "commandHandler.h"
#include "program.h"
#include "token.h"

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
    std::shared_ptr<VarStatement> parseVariableStatement();
    std::shared_ptr<ForStatement> parseForStatement();
    std::shared_ptr<WhileStatement> parseWhileStatement();
    std::shared_ptr<BlockStatement> parseBlockStatement();

    //Syntax error
    String currentPos();
    void displaySyntaxError(const String& commandName);

    // Lexer state
    String input;
    size_t pos;
    Token currentToken;
};