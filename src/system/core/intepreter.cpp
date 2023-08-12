#include "interpreter.h"
#include "os.h"

// Constructor
Interpreter::Interpreter() : pos(0) {
    // Service commands
}

// Process a script
Program Interpreter::processScript(const String& script) {
    input = script;
    pos = 0;
    currentToken = nextToken();

    Program prgm;

    // Parse and execute statements until the end of the script
    while (currentToken.type != END_OF_SCRIPT) {
        prgm.addStatement(parseStatement());
    }
    return prgm;
}


String Interpreter::currentPos(){
    int c = 0;
    int line = 0;
    for (size_t i = 0; i < pos; ++i) {
        if (input.charAt(i) == '\n') {
            line++;
            c = 0;
        }else c++;
    }
    return "[" + String(line) + ":" +  String(c) + "] ";
}

void Interpreter::displaySyntaxError(const String& commandName) {
    for (const auto& info : CommandHandler::getCommands()) {
        if (info.second.syntax.startsWith(commandName)) {
            os.console.error("Interpreter") << "Invalid syntax for " << commandName << ". Expected: " << info.second.syntax << os.console.endl;
            return;
        }
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

    //os.console.info("Interpreter") << "Reading : \"" << ch << "\" at " << currentPos() << os.console.endl;

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
            case '\n' : 
                pos++; // Skip the unexpected character

                return nextToken(); // Recursively call to get the next token
            default:
                // Handle error: unexpected character
                os.console.error("Lexer") << currentPos() << "Unexpected character: " << ch << HERE << os.console.endl;
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

std::shared_ptr<CommandStatement> Interpreter::parseCommandStatement() {
    auto command = std::make_shared<CommandStatement>();
    command->name = currentToken.value;
    currentToken = nextToken(); // Consume the command name

    // Expect an opening parenthesis
    if (currentToken.type != LPAREN) {

    }else{
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
    }

    return command;
}

std::shared_ptr<IfStatement> Interpreter::parseIfStatement() {
    

    currentToken = nextToken(); // Consume the 'if' keyword

    // Parse the condition
    String condition;
    while (currentToken.type != EOL && currentToken.type != END_OF_SCRIPT) {
        condition += currentToken.value;
        currentToken = nextToken();
    }

    os.console.println(condition);
    auto ifStmt = std::make_shared<IfStatement>(condition);
    currentToken = nextToken(); // Consume the condition

    // Parse the true branch
    while (currentToken.type != ELSE && currentToken.type != END && currentToken.type != END_OF_SCRIPT) {
        ifStmt->trueBranch.push_back(parseStatement());
    }

    // Parse the false branch (if present)
    if (currentToken.type == ELSE) {
        currentToken = nextToken(); // Consume the 'else' keyword
        while (currentToken.type != END && currentToken.type != END_OF_SCRIPT) {
            ifStmt->falseBranch.push_back(parseStatement());
        }
    }

    if (currentToken.type != END) {
        // Handle error
        os.console.error("Interpreter") << "Expected 'end', found : " << currentToken.value << HERE << os.console.endl;
        return nullptr; // Return or handle the error as needed
    }
    currentToken = nextToken(); // Consume the 'end' keyword
    return ifStmt;
}

std::shared_ptr<Statement> Interpreter::parseStatement() {
    if (currentToken.type == IDENTIFIER) {
        return parseCommandStatement();
    } else if (currentToken.type == IF) {
        return parseIfStatement();
    } else {
        // Handle error: unexpected token
        os.console.error("Parser") << currentPos() << "Unexpected token: " << currentToken.value << HERE << os.console.endl;
    }

    return nullptr; // Should never reach here
}
