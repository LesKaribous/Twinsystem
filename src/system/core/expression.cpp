#include "expression.h"
#include "os.h"

std::map<String, String> Expression::variables; // A mapping from variable names to values

Expression::Expression(const String& input) : input(input), pos(0) {
     consumeToken(); // Initialize the current token
    root = parseExpression(); // Parse the expression and store the AST in the root member variable
}

String Expression::evaluate() {
    return evaluateNode(root); // Evaluate the precomputed AST
}

void Expression::registerVariables(const String& varname, const String& description) {
    variables[varname] = description;
}


std::shared_ptr<Node> Expression::parseExpression() {
    std::shared_ptr<Node> left = parseTerm();

    while (currentTokenIs(ADD) || currentTokenIs(SUBTRACT) || currentTokenIs(GREATER) || currentTokenIs(LESS) || currentTokenIs(EQUAL) || currentTokenIs(NOT_EQUAL) || currentTokenIs(GREATER_EQUAL) || currentTokenIs(LESS_EQUAL)) {
        TokenType op;
        if (currentTokenIs(ADD)) {
            op = ADD;
        } else if (currentTokenIs(SUBTRACT)) {
            op = SUBTRACT;
        } else if (currentTokenIs(GREATER)) {
            op = GREATER;
        } else if (currentTokenIs(LESS)) {
            op = LESS;
        } else if (currentTokenIs(EQUAL)) {
            op = EQUAL;
        } else if (currentTokenIs(NOT_EQUAL)) {
            op = NOT_EQUAL;
        } else if (currentTokenIs(GREATER_EQUAL)) {
            op = GREATER_EQUAL;
        } else if (currentTokenIs(LESS_EQUAL)) {
            op = LESS_EQUAL;
        }

        consumeToken(); // Move to the next token
        std::shared_ptr<Node> right = parseTerm();
        left = std::make_shared<Node>(op, "", left, right);
    }

    return left;
}

std::shared_ptr<Node> Expression::parseTerm() {
    std::shared_ptr<Node> left = parseFactor();

    while (currentTokenIs(MULTIPLY) || currentTokenIs(DIVIDE)) {
        TokenType op = currentTokenIs(MULTIPLY) ? MULTIPLY : DIVIDE;
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
    } else if (currentTokenIs(LBRACKET)) {
        node = parseVector();
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

    return node;
}
std::shared_ptr<Node> Expression::parseVector() {
    consumeToken(); // Consume the '['

    String x = currentTokenValue();
    consumeToken(); // Consume the x value

    if (!currentTokenIs(COMMA)) {
        // Handle error: missing comma
    }
    consumeToken(); // Consume the ','

    String y = currentTokenValue();
    consumeToken(); // Consume the y value

    if (currentTokenIs(COMMA)) {
        consumeToken(); // Consume the ','

        String z = currentTokenValue();
        consumeToken(); // Consume the z value

        if (!currentTokenIs(RBRACKET)) {
            // Handle error: missing closing bracket
        }
        consumeToken(); // Consume the ']'

        auto node = std::make_shared<Node>();
        node->type = VECTOR3;
        node->value = "[" + x + "," + y + "," + z + "]";
        return node;
    } else if (currentTokenIs(RBRACKET)) {
        consumeToken(); // Consume the ']'

        auto node = std::make_shared<Node>();
        node->type = VECTOR2;
        node->value = "[" + x + "," + y + "]";
        return node;
    } else {
        // Handle error: unexpected token
        os.console.error("Expression") << "Unexpected token while reading vector" << os.console.endl;
        return nullptr;
    }
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
    // ... [rest of the function remains unchanged]

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
    } else if (ch == '>') {
        if (input.charAt(pos + 1) == '=') {
            currentToken = {GREATER_EQUAL, ">="};
            pos += 2;
        } else {
            currentToken = {GREATER, ">"};
            pos++;
        }
    } else if (ch == '<') {
        if (input.charAt(pos + 1) == '=') {
            currentToken = {LESS_EQUAL, "<="};
            pos += 2;
        } else {
            currentToken = {LESS, "<"};
            pos++;
        }
    } else if (ch == '=' && input.charAt(pos + 1) == '=') {
        currentToken = {EQUAL, "=="};
        pos += 2;
    } else if (ch == '!' && input.charAt(pos + 1) == '=') {
        currentToken = {NOT_EQUAL, "!="};
        pos += 2;
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
    } else if (ch == '[') {
        currentToken = {LBRACKET, "["};
        pos++;
    } else if (ch == ']') {
        currentToken = {RBRACKET, "]"};
        pos++;
    } else if (ch == ',') {
        currentToken = {COMMA, ","};
        pos++;
    }else {
        // Handle error: unexpected character
        // ... error handling code ...
    }
}


String Expression::evaluateNode(std::shared_ptr<Node> node) {
    switch (node->type) {
        case LITERAL:       return node->value;
        case VARIABLE:      return evaluateVariable(node);  // Assuming you have a function to evaluate variables
        case VECTOR2:
        case VECTOR3:       return node->value;  // Return the vector as-is
        case ADD:           return evaluateAdd(node);
        case SUBTRACT:      return evaluateSubtract(node);
        case MULTIPLY:      return evaluateMultiply(node);
        case DIVIDE:        return evaluateDivide(node);
        case AND:           return evaluateAnd(node);
        case OR:            return evaluateOr(node);
        case EQUAL:         return evaluateEqual(node);
        case NOT_EQUAL:     return evaluateNotEqual(node);
        case NOT:           return evaluateNot(node);
        case LESS_EQUAL:    return evaluateLessEqual(node);
        case GREATER_EQUAL: return evaluateGreaterEqual(node);
        case LESS:          return evaluateLess(node);
        case GREATER:       return evaluateGreater(node);
        default:
            os.console.error("Expression") << "Unknown node type during evaluation" << os.console.endl;
            return "";  // Return an empty string or handle the error as needed
    }
}


VectorType Expression::getVectorType(const String& value) {
    if (!isVector(value)) {
        return NOT_VECTOR;
    }

    int commaCount = std::count(value.begin(), value.end(), ',');

    if (commaCount == 1) {
        return VEC2;
    } else if (commaCount == 2) {
        return VEC3;
    } else {
        return NOT_VECTOR;  // This might be an error case if you don't expect other vector types.
    }
}



bool Expression::isVector(const String& value) {
    return value.startsWith("[") && value.endsWith("]");
}

String Expression::evaluateAdd(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);

    // Check if either operand is a vector
    if (isVector(leftValue) || isVector(rightValue)) {
        return addVectorsOrScalar(leftValue, rightValue);
    } else {
        // Assuming both are scalars
        float result = leftValue.toFloat() + rightValue.toFloat();
        return String(result);
    }
}

String Expression::addVectorsOrScalar(const String& left, const String& right) {
    VectorType leftType = getVectorType(left);
    VectorType rightType = getVectorType(right);

    if (leftType == VEC2 && rightType == VEC2) {
        return String(Vec2::fromString(left) + Vec2::fromString(right));
    } else if (leftType == VEC3 && rightType == VEC3) {
        return String(Vec3::fromString(left) + Vec3::fromString(right));
    } else {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to addVectorsOrScalar" << os.console.endl;
        return "";
    }
}

String Expression::evaluateSubtract(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);

    // Check if either operand is a vector
    if (isVector(leftValue) || isVector(rightValue)) {
        return subVectorsOrScalar(leftValue, rightValue);
    } else {
        // Assuming both are scalars
        float result = leftValue.toFloat() - rightValue.toFloat();
        return String(result);
    }
}

String Expression::subVectorsOrScalar(const String& left, const String& right) {
    VectorType leftType = getVectorType(left);
    VectorType rightType = getVectorType(right);

    if (leftType == VEC2 && rightType == VEC2) {
        return String(Vec2::fromString(left) - Vec2::fromString(right));
    } else if (leftType == VEC3 && rightType == VEC3) {
        return String(Vec3::fromString(left) - Vec3::fromString(right));
    } else {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to addVectorsOrScalar" << os.console.endl;
        return "";
    }
}

String Expression::evaluateMultiply(std::shared_ptr<Node> node) {
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);

    // Check if either operand is a vector
    if (isVector(leftValue) || isVector(rightValue)) {
        return multVectorsOrScalar(leftValue, rightValue);
    } else {
        // Assuming both are scalars
        float result = leftValue.toFloat() * rightValue.toFloat();
        return String(result);
    }
}

String Expression::multVectorsOrScalar(const String& left, const String& right) {
    VectorType leftType = getVectorType(left);
    VectorType rightType = getVectorType(right);

    if (leftType == VEC2 && rightType == VEC2) {
        return String(Vec2::fromString(left) * Vec2::fromString(right));
    } else if (leftType == VEC3 && rightType == VEC3) {
        return (Vec3::fromString(left) * Vec3::fromString(right));
    } else if (leftType == VEC2 || rightType == VEC2) {
        // One of them is a Vec2 and the other is a scalar
        if(leftType == VEC2) return String( Vec2::fromString(left) * right.toFloat() );
        else if(rightType == VEC2) return String( Vec2::fromString(right) * left.toFloat() );
    } else if (leftType == VEC3 || rightType == VEC3) {
        if(leftType == VEC3) return String( Vec3::fromString(left) * right.toFloat() );
        else if(rightType == VEC3) return String( Vec3::fromString(right) * left.toFloat() );
    } else {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to multVectorsOrScalar" << os.console.endl;
        return "";
    }
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

String Expression::divVectorsOrScalar(const String& left, const String& right) {
    VectorType leftType = getVectorType(left);
    VectorType rightType = getVectorType(right);

    if (leftType == VEC2 && rightType == VEC2) {
        return String(Vec2::fromString(left) / Vec2::fromString(right));
    } else if (leftType == VEC3 && rightType == VEC3) {
        return (Vec3::fromString(left) / Vec3::fromString(right));
    } else if (leftType == VEC2 || rightType == VEC2) {
        // One of them is a Vec2 and the other is a scalar
        if(leftType == VEC2) return String( Vec2::fromString(left) / right.toFloat() );
        else if(rightType == VEC2) return String( Vec2::fromString(right) / left.toFloat() );
    } else if (leftType == VEC3 || rightType == VEC3) {
        if(leftType == VEC3) return String( Vec3::fromString(left) / right.toFloat() );
        else if(rightType == VEC3) return String( Vec3::fromString(right) / left.toFloat() );
    } else {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to multVectorsOrScalar" << os.console.endl;
        return "";
    }
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