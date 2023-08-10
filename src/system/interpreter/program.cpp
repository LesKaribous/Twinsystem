#include "program.h"
#include "os.h"

void Statement::run(){

}

void Statement::reset(){

}

void Statement::start(){

}

void Statement::pause(){

}

void Statement::resume(){

}

void Statement::cancel(){

}

void Statement::complete(){

}






void Program::executeCommand(const CommandStatement& command) {
    // Construct the arguments string
    String args;
    for (size_t i = 0; i < command.arguments.size(); ++i) {
        args += command.arguments[i];
        if (i < command.arguments.size() - 1) {
            args += ',';
        }
    }

    // Call the execute method on the CommandHandler object
    commandHandler.execute(command.name, args);
}

void Program::executeIfStatement(const IfStatement& ifStmt) {
    bool conditionResult = evaluateCondition(ifStmt.condition);

    const auto& branch = conditionResult ? ifStmt.trueBranch : ifStmt.falseBranch;

    for (const auto& statement : branch) {
        executeStatement(statement);
    }
}

bool Program::evaluateCondition(const String& condition) {
    if (condition == "hasObject") {
        return commandHandler.execute_testTRUE();
    } // ... Handle other conditions ...

    // Handle error if condition is not recognized
    return false;
}

void Program::executeStatement(const std::shared_ptr<Statement>& statement) {
    switch (statement->type) {
        case COMMAND_STATEMENT:
            executeCommand(*static_cast<CommandStatement*>(statement.get()));
            break;
        case IF_STATEMENT:
            executeIfStatement(*static_cast<IfStatement*>(statement.get()));
            break;
        // Handle error: unexpected statement type
        default:
            os.console.error("Interpreter")  << "Unexpected statement type" << HERE << os.console.endl;
    }
}

void Program::run(){
    if(_currentTask && isPending()){
        if(_currentTask->isCompleted()){
            step();
        }else if(_currentTask->isCancelled()){
            os.console.error("Program") << "Current task has been cancelled. Stopping program" << os.console.endl;
        }
    }
}

void Program::reset(){
    for(std::shared_ptr<Statement>& st : _statements) st->reset();
    Job::reset();
}

void Program::start(){
    if(_statements.size() > 0){
        Job::start();
        _currentTask = 0;
    }

}

void Program::pause(){

}

void Program::resume(){

}

void Program::cancel(){

}

void Program::complete(){

}


void Program::restart(){

}

void Program::stop(){

}

void Program::step(){

}
