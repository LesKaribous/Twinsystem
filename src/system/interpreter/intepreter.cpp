#include "interpreter.h"

// Constructor
Interpreter::Interpreter() : pos(0) {
}

// Process a script
void Interpreter::processScript(const String& script) {
    input = script;
    pos = 0;
    currentToken = nextToken();

    // Parse and execute statements until the end of the script
    while (currentToken.type != END_OF_SCRIPT) {
        std::unique_ptr<Statement> statement = parseStatement();
        executeStatement(statement);
    }
}

void Interpreter::executeCommand(const CommandStatement& command) {
    CommandHandler handler;

    if (command.name == "enable") {
        handler.execute_enable(command.arguments[0]);
    } else if (command.name == "setAbsPosition") {
        handler.execute_setAbsPosition(command.arguments[0].toFloat(), command.arguments[1].toFloat(), command.arguments[2].toFloat());
    } else if (command.name == "go") {
        handler.execute_go(command.arguments[0].toFloat(), command.arguments[1].toFloat());
    } // ... Handle other commands ...

    // Handle error if command name is not recognized
}

void Interpreter::executeIfStatement(const IfStatement& ifStmt) {
    bool conditionResult = evaluateCondition(ifStmt.condition);

    const auto& branch = conditionResult ? ifStmt.trueBranch : ifStmt.falseBranch;

    for (const auto& statement : branch) {
        executeStatement(statement);
    }
}

bool Interpreter::evaluateCondition(const String& condition) {
    CommandHandler handler;

    if (condition == "hasObject") {
        return handler.execute_testTRUE();
    } // ... Handle other conditions ...

    // Handle error if condition is not recognized
    return false;
}

void Interpreter::executeStatement(const std::unique_ptr<Statement>& statement) {
    switch (statement->type) {
        case COMMAND_STATEMENT:
            executeCommand(*static_cast<CommandStatement*>(statement.get()));
            break;
        case IF_STATEMENT:
            executeIfStatement(*static_cast<IfStatement*>(statement.get()));
            break;
        // Handle error: unexpected statement type
    }
}



// Get the next token from the input
Token Interpreter::nextToken() {
    // Skip any whitespace
    skipWhitespace();

    // Check for the end of the input
    if (pos >= input.length()) {
        return {END_OF_SCRIPT, ""};
    }

    // Get the next character
    char ch = input.charAt(pos);

    // Handle different token types
    if (isDigit(ch)) {
        return parseNumber();
    } else if (isAlpha(ch)) {
        return parseIdentifier();
    } else {
        switch (ch) {
            case '(': pos++; return {LPAREN, "("};
            case ')': pos++; return {RPAREN, ")"};
            case ',': pos++; return {COMMA, ","};
            case '#': // Handle comments by skipping to the end of the line
                while (pos < input.length() && input.charAt(pos) != '\n') {
                    pos++;
                }
                return nextToken(); // Recursively call to get the next token
            // ... Handle other special characters or operators as needed ...
            default:
                // Handle error: unexpected character
                pos++; // Skip the unexpected character
                return nextToken(); // Recursively call to get the next token
        }
    }

    // Return an error token if no match is found
    return {END_OF_SCRIPT, ""};
}

// Skip whitespace in the input
void Interpreter::skipWhitespace() {
    while (pos < input.length() && isWhitespace(input.charAt(pos))) {
        pos++;
    }
}

// Parse a number token (including floating-point numbers)
Token Interpreter::parseNumber() {
    String value;
    while (pos < input.length() && isDigit(input.charAt(pos))) {
        value += input.charAt(pos++);
    }

    // Check for a decimal point
    if (pos < input.length() && input.charAt(pos) == '.') {
        value += input.charAt(pos++); // Consume the '.'

        // Parse the digits after the decimal point
        while (pos < input.length() && isDigit(input.charAt(pos))) {
            value += input.charAt(pos++);
        }
    }

    return {NUMBER, value};
}

// Parse an identifier or keyword token
Token Interpreter::parseIdentifier() {
    String value;
    while (pos < input.length() && (isAlpha(input.charAt(pos)) || isDigit(input.charAt(pos)))) {
        value += input.charAt(pos++);
    }

    // Check for keywords
    if (value == "if") return {IF, value};
    if (value == "else") return {ELSE, value};
    if (value == "end") return {END, value};

    return {IDENTIFIER, value};
}

std::unique_ptr<CommandStatement> Interpreter::parseCommandStatement() {
    auto command = std::make_unique<CommandStatement>();
    command->name = currentToken.value;
    currentToken = nextToken(); // Consume the command name

    // Expect an opening parenthesis
    if (currentToken.type != LPAREN) {
        // Handle error
    }
    currentToken = nextToken(); // Consume the '('

    // Parse the arguments
    while (currentToken.type != RPAREN) {
        if (currentToken.type == NUMBER || currentToken.type == IDENTIFIER) {
            command->arguments.push_back(currentToken.value);
            currentToken = nextToken(); // Consume the argument
        }

        // Handle commas between arguments
        if (currentToken.type == COMMA) {
            currentToken = nextToken(); // Consume the ','
        }
    }

    currentToken = nextToken(); // Consume the ')'

    return command;
}

std::unique_ptr<IfStatement> Interpreter::parseIfStatement() {
    auto ifStmt = std::make_unique<IfStatement>();

    currentToken = nextToken(); // Consume the 'if' keyword

    // Parse the condition (for simplicity, we'll assume it's an identifier)
    if (currentToken.type != IDENTIFIER) {
        // Handle error
    }
    ifStmt->condition = currentToken.value;
    currentToken = nextToken(); // Consume the condition

    // Parse the true branch
    while (currentToken.type != ELSE && currentToken.type != END) {
        ifStmt->trueBranch.push_back(parseStatement());
    }

    // Parse the false branch (if present)
    if (currentToken.type == ELSE) {
        currentToken = nextToken(); // Consume the 'else' keyword
        while (currentToken.type != END) {
            ifStmt->falseBranch.push_back(parseStatement());
        }
    }

    if (currentToken.type != END) {
        // Handle error
    }
    currentToken = nextToken(); // Consume the 'end' keyword

    return ifStmt;
}

std::unique_ptr<Statement> Interpreter::parseStatement() {
    if (currentToken.type == IDENTIFIER) {
        return parseCommandStatement();
    } else if (currentToken.type == IF) {
        return parseIfStatement();
    } else {
        // Handle error: unexpected token
    }

    return nullptr; // Should never reach here
}
