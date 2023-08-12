#include "expression.h"
#include "os.h"

std::map<String, String> Expression::variables; // A mapping from variable names to values

Expression::Expression(const String& input) : input(input), pos(0) {
    root = parseExpression(); // Parse the expression and store the AST in the root member variable
}

String Expression::evaluate() {
    return evaluateNode(root); // Evaluate the precomputed AST
}

void Expression::registerVariables(const String& varname, const String& description) {
    variables[varname] = description;
}


std::shared_ptr<Node> Expression::parseExpression() {
    std::shared_ptr<Node> left = parseFactor();

    while (currentTokenIs(ADD) || currentTokenIs(SUBTRACT)) {
        TokenType op = currentTokenIs(ADD) ? ADD : SUBTRACT;
        consumeToken(); // Move to the next token
        std::shared_ptr<Node> right = parseFactor();
        left = std::make_shared<Node>(op, "", left, right);
    }

    return left;
}

std::shared_ptr<Node> Expression::parseFactor() {
    std::shared_ptr<Node> node;

    if (currentTokenIs(LITERAL)) {
        node = parseLiteral();
    } else if (currentTokenIs(VARIABLE)) {
        node = parseVariable();
    } else if (currentTokenIs(LPAREN)) {
        consumeToken(); // Consume the '('
        node = parseExpression();
        if (!currentTokenIs(RPAREN)) {
            // Handle error: missing closing parenthesis
        }
        consumeToken(); // Consume the ')'
    } else {
        // Handle error: unexpected token
        os.console.error("Expression") << "Unexpected token while reading expression factor" << os.console.endl;
    }

    while (currentTokenIs(MULTIPLY) || currentTokenIs(DIVIDE)) {
        TokenType op = currentTokenIs(MULTIPLY) ? MULTIPLY : DIVIDE;
        consumeToken(); // Move to the next token
        std::shared_ptr<Node> right = parseFactor(); // Recursively parse the next factor
        node = std::make_shared<Node>(op, "", node, right);
    }

    return node;
}

std::shared_ptr<Node> Expression::parseLiteral() {
    String value = currentTokenValue();
    consumeToken(); // Move to the next token
    return std::make_shared<Node>(LITERAL, value, nullptr, nullptr);
}

std::shared_ptr<Node> Expression::parseVariable() {
    String value = currentTokenValue();
    consumeToken(); // Move to the next token
    return std::make_shared<Node>(VARIABLE, value, nullptr, nullptr);
}

bool Expression::currentTokenIs(TokenType type) {
    return currentToken.type == type;
}

String Expression::currentTokenValue() {
    return currentToken.value;
}

void Expression::consumeToken() {
    // Skip whitespace
    while (pos < input.length() && isspace(input.charAt(pos))) {
        pos++;
    }

    // Check for the end of the input
    if (pos >= input.length()) {
        currentToken = {END_OF_EXPRESSION, ""};
        return;
    }

    char ch = input.charAt(pos);

    // Recognize operators and keywords
    if (ch == '+') {
        currentToken = {ADD, "+"};
        pos++;
    } else if (ch == '-') {
        currentToken = {SUBTRACT, "-"};
        pos++;
    } else if (ch == '*') {
        currentToken = {MULTIPLY, "*"};
        pos++;
    } else if (ch == '/') {
        currentToken = {DIVIDE, "/"};
        pos++;
    } else if (isalpha(ch)) {
        // Recognize keywords and variables
        String value;
        while (pos < input.length() && (isalpha(input.charAt(pos)) || isdigit(input.charAt(pos)))) {
            value += input.charAt(pos++);
        }
        if (value == "AND") {
            currentToken = {AND, value};
        } else if (value == "OR") {
            currentToken = {OR, value};
        } // ... other keywords and variables ...
    } else if (isdigit(ch)) {
        // Recognize numeric literals
        String value;
        while (pos < input.length() && isdigit(input.charAt(pos))) {
            value += input.charAt(pos++);
        }
        currentToken = {LITERAL, value};
    } else if (ch == '(') {
        currentToken = {LPAREN, "("};
        pos++;
    } else if (ch == ')') {
        currentToken = {RPAREN, ")"};
        pos++;
    } else {
        // Handle error: unexpected character
        // ... error handling code ...
    }
}

String Expression::evaluateNode(std::shared_ptr<Node> node) {
    if (!node) return ""; // Handle null nodes

    switch (node->type) {
        case ADD: return evaluateAdd(node);
        case SUBTRACT: return evaluateSubtract(node);
        case MULTIPLY: return evaluateMultiply(node);
        case DIVIDE: return evaluateDivide(node);
        case AND: return evaluateAnd(node);
        case OR: return evaluateOr(node);
        case EQUAL: return evaluateEqual(node);
        case NOT_EQUAL: return evaluateNotEqual(node);
        case NOT: return evaluateNot(node);
        case LESS_EQUAL: return evaluateLessEqual(node);
        case GREATER_EQUAL: return evaluateGreaterEqual(node);
        case LESS: return evaluateLess(node);
        case GREATER: return evaluateGreater(node);
        case LITERAL: return node->value; // Return the value of literal nodes
        case VARIABLE: return evaluateVariable(node); // Handle variables
        default: return ""; // Handle error: unknown node type
    }
}

String Expression::evaluateAdd(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    float result = leftValue.toFloat() + rightValue.toFloat();
    return String(result);
}

String Expression::evaluateSubtract(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    float result = leftValue.toFloat() - rightValue.toFloat();
    return String(result);
}

String Expression::evaluateMultiply(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    float result = leftValue.toFloat() * rightValue.toFloat();
    return String(result);
}

String Expression::evaluateDivide(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    float divisor = rightValue.toFloat();
    if (divisor == 0.0f) {
        // Handle error: division by zero
        return "Error: Division by zero";
    }
    float result = leftValue.toFloat() / divisor;
    return String(result);
}


String Expression::evaluateOr(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue == "true") || (rightValue == "true");
    return result ? "true" : "false";
}

String Expression::evaluateAnd(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue == "true") && (rightValue == "true");
    return result ? "true" : "false";
}

String Expression::evaluateNot(std::shared_ptr<Node> node) {
    String value = evaluateNode(node->left); // Assuming the NOT operation is unary and applies to the left child
    bool result = (value != "true");
    return result ? "true" : "false";
}


String Expression::evaluateEqual(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue == rightValue);
    return result ? "true" : "false";
}

String Expression::evaluateNotEqual(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue != rightValue);
    return result ? "true" : "false";
}

String Expression::evaluateLess(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue.toFloat() < rightValue.toFloat());
    return result ? "true" : "false";
}

String Expression::evaluateGreater(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue.toFloat() > rightValue.toFloat());
    return result ? "true" : "false";
}

String Expression::evaluateLessEqual(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue.toFloat() <= rightValue.toFloat());
    return result ? "true" : "false";
}

String Expression::evaluateGreaterEqual(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue.toFloat() >= rightValue.toFloat());
    return result ? "true" : "false";
}

String Expression::evaluateVariable(std::shared_ptr<Node> node) {
    String variableName = node->value;

    // Look up the value of the variable based on its name
    // This will depend on how variables are stored in your system
    String value = lookupVariableValue(variableName);

    return value;
}

String Expression::lookupVariableValue(const String& variableName) {
    auto it = variables.find(variableName);
    if (it != variables.end()) {
        return it->second; // Return the value of the variable
    } else {
        // Handle error: undefined variable
        os.console.error("Expression") << "Error: Undefined variable " + variableName << os.console.endl;
        return "error";
    }
}