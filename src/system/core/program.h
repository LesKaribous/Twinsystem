#pragma once
#include "system/core/job.h"
#include "system/core/lib.h"
#include "commandHandler.h"
#include "expression.h"

enum StatementType { IGNORED_STATEMENT, COMMAND_STATEMENT, IF_STATEMENT, VAR_STATEMENT, FOR_STATEMENT, WHILE_STATEMENT, BLOCK_STATEMENT };
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
    IfStatement(const String& exp) : condition(exp){ type = IF_STATEMENT; }
    Expression condition;
    std::vector<std::shared_ptr<Statement>> trueBranch;
    std::vector<std::shared_ptr<Statement>> falseBranch;
};

struct VarStatement : public Statement{
    VarStatement(const String& e) : exp(e){ type = VAR_STATEMENT; }
    Expression exp;
};

struct ForStatement : public Statement{
    ForStatement(const String& varExp, const String& condExp, const String& stepExp) : var(varExp), condition(condExp), step(stepExp){ type = FOR_STATEMENT; }

    VarStatement var;
    Expression condition;
    Expression step;

    std::vector<std::shared_ptr<Statement>> content;
};

struct WhileStatement : public Statement{
    WhileStatement(const String& exp) : condition(exp){ type = WHILE_STATEMENT; }
    Expression condition;
    std::vector<std::shared_ptr<Statement>> content;
};

struct BlockStatement : public Statement{
    BlockStatement(const String& exp) : condition(exp){ type = BLOCK_STATEMENT; }
    Expression condition;
    std::vector<std::shared_ptr<Statement>> content;
};


class Program : public Job {
    std::vector<std::shared_ptr<Statement>> _statements;

    size_t _currentTask = 0;

    CommandHandler commandHandler;

    // Execution functions
    void executeStatement(const std::shared_ptr<Statement>& statement);
    void executeCommand(const CommandStatement&);
    void executeIfStatement(IfStatement&);
    void executeForStatement(ForStatement&);
    void executeWhileStatement(WhileStatement&);
    void executeBlockStatement(BlockStatement&);
    void executeVarStatement(VarStatement&);

    // Evaluation functions
    String evaluateExpression(const String& e);
    String evaluateExpression(Expression& expression);
    
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