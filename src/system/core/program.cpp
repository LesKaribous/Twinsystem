#include "program.h"
#include "os.h"


void Program::executeCommand(const CommandStatement& command) {
    // Construct the arguments string
    os.console.trace("Program") << "Running command : " << command.name << "( " << int(command.arguments.size()) << " )" << os.console.endl;
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


String symbols[9]{
    "&&",
    "||",
    "==",
    "!=",
    "!",
    "<=",
    ">=",
    "<",
    ">"
};

int findClosestOperator(const String& str, int index, String& buf){
    int min = str.length();
    //THROW(str)
    for(String test : symbols){
        int r = str.indexOf(test, index);
        if(r < min && r != -1){
            min = r;
            buf = test;
        }
    }

    if((unsigned int) min == str.length()) return -1;
    else return min;
}

void parseCondition(const String& raw, std::vector<String>& args, std::vector<String>& operators){
    // Count the operators in condition
    int i = 0;
    int argc = 1;

    String operatorBuf;
    while(true){
        i = findClosestOperator(raw, i, operatorBuf);
        if(i != -1){
            operators.push_back(operatorBuf);
            if(operatorBuf != "!") argc++;
            i++;
        }else break;
        
    }; 

    String subC = raw;
    for(String test : symbols){
        if(test == "!") subC.replace(test, "");
        else subC.replace(test, "/");
    }
    subC.replace(" ", "");

    int start = 0;
    int end = subC.indexOf('/', start);
    
    // Find the start and end position of the argument at the specified index
    for (int i = 0; i < argc; i++) {
        if (end != -1){
            args.push_back(subC.substring(start, end).trim());
            start = end + 1;
            end = subC.indexOf('/', start);
        }

    }
    args.push_back(subC.substring(start, end).trim());
}

bool Program::evaluateCondition(const String& condition) {

    std::vector<String> args;
    std::vector<String> operators;

    // Parse the condition into args and operators
    parseCondition(condition, args, operators);

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
    //Do nothing
}

void Program::reset(){
    Job::reset();
}

void Program::start(){
    if(_statements.size() > 0){
        Job::start();
        _currentTask = 0;
        executeStatement(_statements[_currentTask]);
    }

}

void Program::pause(){
    Job::pause();
}

void Program::resume(){
    Job::resume();
}

void Program::cancel(){
    Job::cancel();
}

void Program::complete(){
    Job::complete();
}


void Program::restart(){
    stop();
    start();
}

void Program::stop(){
    cancel();
    reset();
}

bool Program::step(){
    _currentTask++;
    if(_currentTask >= _statements.size()){
        complete();
        _currentTask = 0; //for safety
        return false;
    }else{
        executeStatement(_statements[_currentTask]);
    }
    return true;
}
