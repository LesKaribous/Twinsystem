#pragma once 
#include "utils/commandHandler.h"
#include "program.h"
#include "token.h"

class Interpreter {
public:
    Interpreter();
    void processScript(const String& script, Program& prgm);

private:
    // Lexer functions
    Token nextToken();
    Token parseNumber();
    Token parseIdentifier();

    String untilLineEnd();

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
