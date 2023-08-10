#pragma once 
#include "system/core/lib.h"
#include "commandHandler.h"

// Token types
enum TokenType { COMMAND, LPAREN, RPAREN, COMMA, NUMBER, IDENTIFIER, IF, ELSE, END, COMMENT, EOL, END_OF_SCRIPT };

// Token structure
struct Token {
    TokenType type;
    String value;
};


enum StatementType { COMMAND_STATEMENT, IF_STATEMENT };
// Base class for statements
struct Statement {
    StatementType type;
    virtual ~Statement() = default;
};

struct CommandStatement : Statement {
    CommandStatement() { type = COMMAND_STATEMENT; }
    String name;
    std::vector<String> arguments;
};

struct IfStatement : Statement {
    IfStatement() { type = IF_STATEMENT; }
    String condition;
    std::vector<std::unique_ptr<Statement>> trueBranch;
    std::vector<std::unique_ptr<Statement>> falseBranch;
};

class Interpreter {
public:
    Interpreter();
    void processScript(const String& script);

private:
    // Lexer functions
    Token nextToken();
    Token parseNumber();
    Token parseIdentifier();
    void skipWhitespace();

    // Parser functions
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<CommandStatement> parseCommandStatement();
    std::unique_ptr<IfStatement> parseIfStatement();

    // Execution functions
    void executeStatement(const std::unique_ptr<Statement>& statement);
    void executeCommand(const CommandStatement& command);
    void executeIfStatement(const IfStatement& ifStmt);

    // Evaluation functions
    bool evaluateCondition(const String& condition);
    float evaluateExpression(const String& expression);

    // Lexer state
    String input;
    size_t pos;
    Token currentToken;
};
