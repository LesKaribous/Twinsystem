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
    if (value.equalsIgnoreCase("if"))    return {IF, value};
    if (value.equalsIgnoreCase("else"))  return {ELSE, value};
    if (value.equalsIgnoreCase("for"))   return {FOR, value};
    if (value.equalsIgnoreCase("while")) return {WHILE, value};
    if (value.equalsIgnoreCase("block")) return {BLOCK, value};
    if (value.equalsIgnoreCase("var")) return {VAR, value};
    if (value.equalsIgnoreCase("end"))   return {END, value};

    return {IDENTIFIER, value};
}

std::shared_ptr<CommandStatement> Interpreter::parseCommandStatement() {
    auto command = std::make_shared<CommandStatement>();
    command->name = currentToken.value;
    currentToken = nextToken(); // Consume the command name

    // Expect an opening parenthesis
    if (currentToken.type != LPAREN) {
        //error
        os.console.error("Interpreter") << "Expecting '(' got " << currentToken.value << os.console.endl;
    }else{

        String arg;
        while (pos+1 < input.length()) {
            if(input.charAt(pos+1) != '\n' || input.charAt(pos) != ',') arg += input.charAt(pos++);
            else break;
        }
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

    // Parse the condition without parenthethis
    String expression;
    while (pos+1 < input.length()) {
        if(input.charAt(pos+1) != '\n') expression += input.charAt(pos++);
        else break;
    }

    os.console.println(expression);
    auto ifStmt = std::make_shared<IfStatement>(expression);
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

std::shared_ptr<VarStatement> Interpreter::parseVariableStatement() {
    currentToken = nextToken(); // Consume the 'var' keyword
    // Parse the condition without parenthethis
    String expression;
    while (pos+1 < input.length()) {
        if(input.charAt(pos+1) != '\n') expression += input.charAt(pos++);
        else break;
    }

    std::shared_ptr<VarStatement> varPtr = std::make_shared<VarStatement>(expression);
}

std::shared_ptr<ForStatement> Interpreter::parseForStatement() {
    currentToken = nextToken(); // Consume the 'for' keyword

    String varExpression;
    String conditionExpression;
    String stepExpression;

    if(currentToken.type != LPAREN){
        os.console.error("Interpreter") << "Expected '()', found : " << currentToken.value << HERE << os.console.endl;
        return nullptr; // Return or handle the error as needed
    }
    currentToken = nextToken(); // Consume the parenthesis

    if(currentToken.type != VAR){
        os.console.error("Interpreter") << "Expected 'var', found : " << currentToken.value << HERE << os.console.endl;
        return nullptr; // Return or handle the error as needed
    }
    currentToken = nextToken(); // Consume the var
    skipWhitespace();
    while (pos < input.length()) {
        if(input.charAt(pos)!=';')  varExpression += input.charAt(pos++);
        else break;
        skipWhitespace();
    }

    pos++; //skip the ;
    skipWhitespace();
    while (pos < input.length()) {
        if(input.charAt(pos)!=';')  conditionExpression += input.charAt(pos++);
        else break;
        skipWhitespace();
    }
    pos++; //skip the ;
    skipWhitespace();
    while (pos < input.length()) {
        if(input.charAt(pos)!=')')  stepExpression += input.charAt(pos++);
        else break;
        skipWhitespace();
    }


    auto forStmt = std::make_shared<ForStatement>(varExpression, conditionExpression, stepExpression);
    currentToken = nextToken(); // Consume the line

    // Parse the content
    while (currentToken.type != END && currentToken.type != END_OF_SCRIPT) {
        forStmt->content.push_back(parseStatement());
    }

    if (currentToken.type != END) {
        // Handle error
        os.console.error("Interpreter") << "Expected 'end', found : " << currentToken.value << HERE << os.console.endl;
        return nullptr; // Return or handle the error as needed
    }
    currentToken = nextToken(); // Consume the 'end' keyword
    return forStmt;
}

std::shared_ptr<WhileStatement> Interpreter::parseWhileStatement() {
    currentToken = nextToken(); // Consume the 'while' keyword

    // Parse the condition without parenthethis
    String expression;
    while (pos+1 < input.length()) {
        if(input.charAt(pos+1) != '\n') expression += input.charAt(pos++);
        else break;
    }

    os.console.println(expression);
    auto whileStmt = std::make_shared<WhileStatement>(expression);
    currentToken = nextToken(); // Consume the condition

    // Parse the true branch
    while (currentToken.type != END && currentToken.type != END_OF_SCRIPT) {
        whileStmt->content.push_back(parseStatement());
    }

    if (currentToken.type != END) {
        // Handle error
        os.console.error("Interpreter") << "Expected 'end', found : " << currentToken.value << HERE << os.console.endl;
        return nullptr; // Return or handle the error as needed
    }
    currentToken = nextToken(); // Consume the 'end' keyword
    return whileStmt;
}

std::shared_ptr<BlockStatement> Interpreter::parseBlockStatement() {
    currentToken = nextToken(); // Consume the 'block' keyword

    // Parse the condition without parenthethis
    String expression;
    while (pos+1 < input.length()) {
        if(input.charAt(pos+1) != '\n') expression += input.charAt(pos++);
        else break;
    }

    os.console.println(expression);
    auto blockStmt = std::make_shared<BlockStatement>(expression);
    currentToken = nextToken(); // Consume the condition

    // Parse the true branch
    while (currentToken.type != END && currentToken.type != END_OF_SCRIPT) {
        blockStmt->content.push_back(parseStatement());
    }

    if (currentToken.type != END) {
        // Handle error
        os.console.error("Interpreter") << "Expected 'end', found : " << currentToken.value << HERE << os.console.endl;
        return nullptr; // Return or handle the error as needed
    }
    currentToken = nextToken(); // Consume the 'end' keyword
    return blockStmt;
}


std::shared_ptr<Statement> Interpreter::parseStatement() {
    if (currentToken.type == IDENTIFIER) {
        return parseCommandStatement();
    } if (currentToken.type == VAR) {
        return parseVariableStatement();
    } else if (currentToken.type == IF) {
        return parseIfStatement();
    } else if (currentToken.type == FOR) {
        return parseForStatement();
    } else if (currentToken.type == WHILE) {
        return parseWhileStatement();
    } else if (currentToken.type == BLOCK) {
        return parseBlockStatement();
    } else {
        // Handle error: unexpected token
        os.console.error("Parser") << currentPos() << "Unexpected token: " << currentToken.value << HERE << os.console.endl;
    }

    return nullptr; // Should never reach here
}