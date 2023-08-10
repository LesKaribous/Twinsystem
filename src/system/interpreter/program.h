#pragma once
#include "system/core/job.h"
#include "system/core/lib.h"
#include "commandHandler.h"

enum StatementType { IGNORED_STATEMENT, COMMAND_STATEMENT, IF_STATEMENT };
// Base class for statements
class Statement : public Job{
protected :
    StatementType type = IGNORED_STATEMENT;

public :
    friend class Program;
    Statement(){};

    void run()      override; 
    void reset()    override;  
    void start()    override; 
    void pause()    override;
    void resume()   override;
    void cancel()   override;
    void complete() override;

    virtual ~Statement() = default;
};

class CommandStatement : public Statement {
public:
    CommandStatement() { type = COMMAND_STATEMENT; }
    String name;
    std::vector<String> arguments;
};

class IfStatement : public Statement {
public:
    IfStatement() { type = IF_STATEMENT; }
    String condition;
    std::vector<std::shared_ptr<Statement>> trueBranch;
    std::vector<std::shared_ptr<Statement>> falseBranch;
};


class Program : public Job {
    std::vector<std::shared_ptr<Statement>> _statements;
    std::shared_ptr<Statement> _currentTask;

    CommandHandler commandHandler;

    // Execution functions
    void executeStatement(const std::shared_ptr<Statement>& statement);
    void executeCommand(const CommandStatement& command);
    void executeIfStatement(const IfStatement& ifStmt);

    // Evaluation functions
    bool evaluateCondition(const String& condition);
    float evaluateExpression(const String& expression);
    
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

    void restart();
    void stop();

private:

    void step();

    
};