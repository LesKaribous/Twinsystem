#pragma once
#include "system/core/job.h"
#include "system/core/lib.h"
#include "commandHandler.h"

enum StatementType { IGNORED_STATEMENT, COMMAND_STATEMENT, IF_STATEMENT };
// Base class for statements
struct Statement{
    StatementType type = IGNORED_STATEMENT;
    Statement(){};
    virtual ~Statement() = default;
};

struct CommandStatement : public Statement {
    CommandStatement() { type = COMMAND_STATEMENT; }
    String name;
    std::vector<String> arguments;
};

struct IfStatement : public Statement {
    IfStatement() { type = IF_STATEMENT; }
    String condition;
    std::vector<std::shared_ptr<Statement>> trueBranch;
    std::vector<std::shared_ptr<Statement>> falseBranch;
};


class Program : public Job {
    std::vector<std::shared_ptr<Statement>> _statements;

    size_t _currentTask = 0;

    CommandHandler commandHandler;

    // Execution functions
    void executeStatement(const std::shared_ptr<Statement>& statement);
    void executeCommand(const CommandStatement& command);
    void executeIfStatement(const IfStatement& ifStmt);

    // Evaluation functions
    bool evaluateCondition(const String& condition);
    String evaluateExpression(const String& expression);
    
public:

    Program(){};

    inline void addStatement(std::shared_ptr<Statement> statement){_statements.push_back(statement);}
    inline bool isValid(){return _statements.size() > 0;};

    void run()      override; 
    void reset()    override;  
    void start()    override; 
    void pause()    override;
    void resume()   override;
    void cancel()   override;
    void complete() override;

    bool step();
    void restart();
    void stop();
};