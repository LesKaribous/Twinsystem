#include "expression.h"
#include "os.h"

Expression::Expression(const String &str) : pos(0)
{
    input = "";
    for (char c : str) {
        if(c == ' ' && !input.equalsIgnoreCase("VAR")); //remove useless spaces
        else input += c; 
    }
    os.console.println( ">" + input);
    consumeToken();           // Initialize the current token
    root = parseExpression(); // Parse the expression and store the AST in the root member variable
}

void Expression::printCompiled(){
    os.console.print("Compiled Expression : ");
    printCompiledNode(root);
    os.console.println("");
}

void Expression::printCompiledNode(std::shared_ptr<Node> node){
    if(node != nullptr){
        if(node->left != nullptr){
            if(node->left->right.get() != node.get()){
                os.console.print("[");
                printCompiledNode(node->left);
                os.console.print("]>");
            }
            
        }
        
        os.console.print(Token::tokenTypetoString(node->type));
        
        if(node->right != nullptr){
            
            if(node->right->left.get() != node.get()){
                os.console.print("<[");
                printCompiledNode(node->right);
                os.console.print("]");
            }
        }
    }
}

String Expression::evaluate()
{
    return evaluateNode(root); // Evaluate the precomputed AST
}

void Expression::registerVariables(const String &varname, const String &value)
{
    getVariables()[varname] = value;
}

void Expression::setVariable(const String &varname, const String &value)
{
    getVariables()[varname] = value;
}

std::map<String, String> &Expression::getVariables()
{
    static std::map<String, String> variables;
    return variables;
}

std::shared_ptr<Node> Expression::parseExpression()
{
    if (currentTokenIs(VAR))
    {   
        consumeToken(); // Consume the 'var' keyword
        if (!currentTokenIs(VARIABLE))
        {
            os.console.error("Expression")  << "expected variable name after 'var'" << os.console.endl;
        }
        String varName = currentTokenValue();
        consumeToken(); // Consume the variable name
        if (!currentTokenIs(ASSIGN))
        {
            // Handle error: expected '=' after variable name
            os.console.warn("Expression")  << "variable " << varName <<  "is undefined" << os.console.endl;
        }
        consumeToken(); // Consume the '='
        std::shared_ptr<Node> variable = std::make_shared<Node>(VARIABLE, varName, nullptr, nullptr);
        std::shared_ptr<Node> valueNode = parseExpression();
        return std::make_shared<Node>(ASSIGN, varName, variable, valueNode);
    }
    else
    {
        std::shared_ptr<Node> left = parseTerm();

        while (currentTokenIs(ASSIGN) || currentTokenIs(ADD) || currentTokenIs(SUBTRACT) || currentTokenIs(GREATER) || currentTokenIs(LESS) || currentTokenIs(EQUAL) || currentTokenIs(NOT_EQUAL) || currentTokenIs(GREATER_EQUAL) || currentTokenIs(LESS_EQUAL))
        {
            TokenType op;
            if (currentTokenIs(ASSIGN))
            {
                op = ASSIGN;
            }
            else if (currentTokenIs(GREATER))
            {
                op = GREATER;
            }
            else if (currentTokenIs(LESS))
            {
                op = LESS;
            }
            else if (currentTokenIs(EQUAL))
            {
                op = EQUAL;
            }
            else if (currentTokenIs(NOT_EQUAL))
            {
                op = NOT_EQUAL;
            }
            else if (currentTokenIs(GREATER_EQUAL))
            {
                op = GREATER_EQUAL;
            }
            else if (currentTokenIs(LESS_EQUAL))
            {
                op = LESS_EQUAL;
            }
            else if (currentTokenIs(ADD))
            {
                op = ADD;
            }
            else if (currentTokenIs(SUBTRACT))
            {
                op = SUBTRACT;
            }
            consumeToken(); // Move to the next token
            std::shared_ptr<Node> right = op == ASSIGN ? parseExpression() : parseTerm();
            left = std::make_shared<Node>(op, "", left, right);
        }
        return left;
    }
}

std::shared_ptr<Node> Expression::parseTerm()
{
    std::shared_ptr<Node> term = parseFactor();

    while (currentTokenIs(MULTIPLY) || currentTokenIs(DIVIDE))
    {
        TokenType op = currentTokenIs(MULTIPLY) ? MULTIPLY : DIVIDE;
        consumeToken(); // Move to the next token
        std::shared_ptr<Node> right = parseFactor();
        term = std::make_shared<Node>(op, "", term, right);
    }

    return term;
}

std::shared_ptr<Node> Expression::parseFactor()
{
    std::shared_ptr<Node> node;

    if (currentTokenIs(LITERAL) || currentTokenIs(SUBTRACT))
    {
        node = parseLiteral();
    }
    else if (currentTokenIs(VARIABLE))
    {
        node = parseVariable();
    }
    else if (currentTokenIs(LBRACKET))
    {
        node = parseVector();
    }
    else if (currentTokenIs(LPAREN))
    {
        consumeToken(); // Consume the '('
        node = parseExpression();
        if (!currentTokenIs(RPAREN))
        {
            // Handle error: missing closing parenthesis
        }
        consumeToken(); // Consume the ')'
    }
    else
    {
        // Handle error: unexpected token
        os.console.error("Expression") << "Unexpected token while reading expression factor" << os.console.endl;
    }

    return node;
}
std::shared_ptr<Node> Expression::parseVector()
{
    consumeToken(); // Consume the '['

    String x = currentTokenValue();
    consumeToken(); // Consume the x value

    if (!currentTokenIs(COMMA))
    {
        // Handle error: missing comma
    }
    consumeToken(); // Consume the ','

    String y = currentTokenValue();
    consumeToken(); // Consume the y value

    if (currentTokenIs(COMMA))
    {
        consumeToken(); // Consume the ','

        String z = currentTokenValue();
        consumeToken(); // Consume the z value

        if (!currentTokenIs(RBRACKET))
        {
            // Handle error: missing closing bracket
        }
        consumeToken(); // Consume the ']'

        auto node = std::make_shared<Node>();
        node->type = VECTOR3;
        node->value = "[" + x + "," + y + "," + z + "]";
        return node;
    }
    else if (currentTokenIs(RBRACKET))
    {
        consumeToken(); // Consume the ']'

        auto node = std::make_shared<Node>();
        node->type = VECTOR2;
        node->value = "[" + x + "," + y + "]";
        return node;
    }
    else
    {
        // Handle error: unexpected token
        os.console.error("Expression") << "Unexpected token while reading vector" << os.console.endl;
        return nullptr;
    }
}

std::shared_ptr<Node> Expression::parseLiteral()
{
    String value = currentTokenValue();
    consumeToken(); // Move to the next token
    return std::make_shared<Node>(LITERAL, value, nullptr, nullptr);
}

std::shared_ptr<Node> Expression::parseVariable()
{
    String value = currentTokenValue();
    consumeToken(); // Move to the next token
    return std::make_shared<Node>(VARIABLE, value, nullptr, nullptr);
}

bool Expression::currentTokenIs(TokenType type)
{
    return currentToken.type == type;
}

String Expression::currentTokenValue()
{
    return currentToken.value;
}

void Expression::consumeToken()
{
    // ... [rest of the function remains unchanged]
    previousToken = currentToken;
    char ch = input.charAt(pos);
    // Recognize operators and keywords
    if (ch == '+')
    {
        currentToken = {ADD, "+"};
        pos++;
    }
    else if (ch == '-')
    {
        // Check for negative numbers
        if (pos + 1 < input.length() && isdigit(input.charAt(pos + 1)) &&
            (previousToken.type == LPAREN || previousToken.type == ADD || previousToken.type == SUBTRACT ||
             previousToken.type == MULTIPLY || previousToken.type == DIVIDE || previousToken.type == NONE ||
             previousToken.type == GREATER || previousToken.type == LESS || previousToken.type == GREATER_EQUAL || previousToken.type == LESS_EQUAL ||
             previousToken.type == EQUAL))
        {
            String value = "-";
            pos++;
            while (pos < input.length() && (isdigit(input.charAt(pos)) || input.charAt(pos) == '.'))
            {
                value += input.charAt(pos++);
            }
            currentToken = {LITERAL, value};
        }
        else
        {
            currentToken = {SUBTRACT, "-"};
            pos++;
        }
    }
    else if (ch == '*')
    {
        currentToken = {MULTIPLY, "*"};
        pos++;
    }
    else if (ch == '/')
    {
        currentToken = {DIVIDE, "/"};
        pos++;
    }
    else if (ch == '>')
    {
        if (input.charAt(pos + 1) == '=')
        {
            currentToken = {GREATER_EQUAL, ">="};
            pos += 2;
        }
        else
        {
            currentToken = {GREATER, ">"};
            pos++;
        }
    }
    else if (ch == '<')
    {
        if (input.charAt(pos + 1) == '=')
        {
            currentToken = {LESS_EQUAL, "<="};
            pos += 2;
        }
        else
        {
            currentToken = {LESS, "<"};
            pos++;
        }
    }
    else if (ch == '=')
    {
        if (input.charAt(pos + 1) == '=')
        {
            currentToken = {EQUAL, "=="};
            pos += 2;
        }
        else
        {
            currentToken = {ASSIGN, "="};
            pos++;
        }
    }
    else if (ch == '!' && input.charAt(pos + 1) == '=')
    {
        currentToken = {NOT_EQUAL, "!="};
        pos += 2;
    }
    else if (isalpha(ch))
    {
        // Recognize keywords and variables
        String value;
        while (pos < input.length() && input.charAt(pos) != ' ' && (isalpha(input.charAt(pos)) || isdigit(input.charAt(pos))))
        {
            value += input.charAt(pos++);
        }

        if (value.equalsIgnoreCase("AND"))
        {
            currentToken = {AND, value};
        }
        else if (value.equalsIgnoreCase("OR"))
        {
            currentToken = {OR, value};
        }
        else if (getVariables().find(value) != getVariables().end())
        {
            currentToken = {VARIABLE, value};
        }else{
            if(previousToken.type != VAR) os.console.warn("Expression") << "Unknown variable or identifier " << value << " used in program" << os.console.endl;
            currentToken = {VARIABLE, value};
        }
    }
    else if (isdigit(ch))
    {
        // Recognize numeric literals
        String value;
        while (pos < input.length() && (isdigit(input.charAt(pos)) || input.charAt(pos) == '.'))
        {
            value += input.charAt(pos++);
        }
        currentToken = {LITERAL, value};
    }
    else if (ch == '(')
    {
        currentToken = {LPAREN, "("};
        pos++;
    }
    else if (ch == ')')
    {
        currentToken = {RPAREN, ")"};
        pos++;
    }
    else if (ch == '[')
    {
        currentToken = {LBRACKET, "["};
        pos++;
    }
    else if (ch == ']')
    {
        currentToken = {RBRACKET, "]"};
        pos++;
    }
    else if (ch == ',')
    {
        currentToken = {COMMA, ","};
        pos++;
    }
    else
    {
        // Handle error: unexpected character
        // ... error handling code ...
    }
    //os.console.println(currentToken.toString());
}

String Expression::evaluateNode(std::shared_ptr<Node> node)
{
    switch (node->type){
    case ASSIGN:
        return evaluateAssign(node);
    case LITERAL:
        return node->value;
    case VARIABLE:
        return evaluateVariable(node); // Assuming you have a function to evaluate variables
    case VECTOR2:
    case VECTOR3:
        return node->value; // Return the vector as-is
    case ADD:
        return evaluateAdd(node);
    case SUBTRACT:
        return evaluateSubtract(node);
    case MULTIPLY:
        return evaluateMultiply(node);
    case DIVIDE:
        return evaluateDivide(node);
    case AND:
        return evaluateAnd(node);
    case OR:
        return evaluateOr(node);
    case EQUAL:
        return evaluateEqual(node);
    case NOT_EQUAL:
        return evaluateNotEqual(node);
    case NOT:
        return evaluateNot(node);
    case LESS_EQUAL:
        return evaluateLessEqual(node);
    case GREATER_EQUAL:
        return evaluateGreaterEqual(node);
    case LESS:
        return evaluateLess(node);
    case GREATER:
        return evaluateGreater(node);
    default:
        os.console.error("Expression") << "Unknown node type during evaluation : " << os.console.endl;
        return ""; // Return an empty string or handle the error as needed
    }
}

bool Expression::isVector(const Node &node)
{
    return node.type == VECTOR2 || node.type == VECTOR3;
}

bool Expression::isValue(const Node &node)
{
    return node.type == VECTOR2 || node.type == VECTOR3 || node.type == LITERAL || node.type == TokenType::VARIABLE;
}

String Expression::evaluateAdd(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    // Check if either operand is a vector
    if (isVector(*node->left) || isVector(*node->right))
    {
        return addVectorsOrScalar(*node->left, *node->right);
    }
    else
    {
        // Assuming both are scalars
        float result = leftValue.toFloat() + rightValue.toFloat();
        return String(result);
    }
}

String Expression::addVectorsOrScalar(const Node &left, const Node &right)
{
    TokenType leftType = left.type;
    TokenType rightType = right.type;

    if (leftType == VECTOR2 && rightType == VECTOR2)
    {
        return String(Vec2::fromString(left.value) + Vec2::fromString(right.value));
    }
    else if (leftType == VECTOR3 && rightType == VECTOR3)
    {
        return String(Vec3::fromString(left.value) + Vec3::fromString(right.value));
    }
    else
    {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to addVectorsOrScalar" << os.console.endl;
        return "";
    }
}

String Expression::evaluateSubtract(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);

    // Check if either operand is a vector
    if (isVector(*node->left) || isVector(*node->right))
    {
        return subVectorsOrScalar(*node->left, *node->right);
    }
    else
    {
        // Assuming both are scalars
        float result = leftValue.toFloat() - rightValue.toFloat();
        return String(result);
    }
}

String Expression::subVectorsOrScalar(const Node &left, const Node &right)
{
    TokenType leftType = left.type;
    TokenType rightType = right.type;

    if (leftType == VECTOR2 && rightType == VECTOR2)
    {
        return String(Vec2::fromString(left.value) - Vec2::fromString(right.value));
    }
    else if (leftType == VECTOR3 && rightType == VECTOR3)
    {
        return String(Vec3::fromString(left.value) - Vec3::fromString(right.value));
    }
    else
    {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to addVectorsOrScalar" << os.console.endl;
        return "";
    }
}

String Expression::evaluateMultiply(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);

    // Check if either operand is a vector
    if (isVector(*node->left) || isVector(*node->right))
    {
        return multVectorsOrScalar(*node->left, *node->right);
    }
    else
    {
        // Assuming both are scalars
        float result = leftValue.toFloat() * rightValue.toFloat();
        return String(result);
    }
}

String Expression::multVectorsOrScalar(const Node &left, const Node &right)
{
    TokenType leftType = left.type;
    TokenType rightType = right.type;

    if (leftType == VECTOR2 && rightType == VECTOR2)
    {
        return String(Vec2::fromString(left.value) * Vec2::fromString(right.value));
    }
    else if (leftType == VECTOR3 && rightType == VECTOR3)
    {
        return (Vec3::fromString(left.value) * Vec3::fromString(right.value));
    }
    else if (leftType == VECTOR2 || rightType == VECTOR2)
    {
        // One of them is a Vec2 and the other is a scalar
        if (leftType == VECTOR2)
            return String(Vec2::fromString(left.value) * right.value.toFloat());
        else if (rightType == VECTOR2)
            return String(Vec2::fromString(right.value) * left.value.toFloat());
    }
    else if (leftType == VECTOR3 || rightType == VECTOR3)
    {
        if (leftType == VECTOR3)
            return String(Vec3::fromString(left.value) * right.value.toFloat());
        else if (rightType == VECTOR3)
            return String(Vec3::fromString(right.value) * left.value.toFloat());
    }
    else
    {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to multVectorsOrScalar" << os.console.endl;
        return "";
    }
}

String Expression::evaluateDivide(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);

    // Check if either operand is a vector
    if (isVector(*node->left) || isVector(*node->right))
    {
        return divVectorsOrScalar(*node->left, *node->right);
    }
    else
    {
        float divisor = rightValue.toFloat();
        if (divisor == 0.0f)
        {
            // Handle error: division by zero
            return "Error: Division by zero";
        }
        float result = leftValue.toFloat() / divisor;
        return String(result);
    }
}

String Expression::divVectorsOrScalar(const Node &left, const Node &right)
{
    TokenType leftType = left.type;
    TokenType rightType = right.type;

    if (leftType == VECTOR2 && rightType == VECTOR2)
    {
        return String(Vec2::fromString(left.value) / Vec2::fromString(right.value));
    }
    else if (leftType == VECTOR3 && rightType == VECTOR3)
    {
        return (Vec3::fromString(left.value) / Vec3::fromString(right.value));
    }
    else if (leftType == VECTOR2)
    {
        // Left is a Vec2 and the other is a scalar
        return String(Vec2::fromString(left.value) / right.value.toFloat());
    }
    else if (leftType == VECTOR3)
    {
        return String(Vec3::fromString(left.value) / right.value.toFloat());
    }
    else
    {
        // Handle error: unexpected input
        os.console.error("Expression") << "Unexpected input to divVectorsOrScalar : " << left.value << " / " << right.value << os.console.endl;
        return "";
    }
}

String Expression::evaluateOr(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue == "true") || (rightValue == "true");
    return result ? "true" : "false";
}

String Expression::evaluateAnd(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue == "true") && (rightValue == "true");
    return result ? "true" : "false";
}

String Expression::evaluateNot(std::shared_ptr<Node> node)
{
    String value = evaluateNode(node->left); // Assuming the NOT operation is unary and applies to the left child
    bool result = (value != "true");
    return result ? "true" : "false";
}

String Expression::evaluateEqual(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue == rightValue);
    return result ? "true" : "false";
}

String Expression::evaluateNotEqual(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = (leftValue != rightValue);
    return result ? "true" : "false";
}

String Expression::evaluateLess(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = false;
    if (isVector(*node->left) || isVector(*node->right))
    {
        os.console.error("Expression") << "Vector comparison is not supported yet." << os.console.endl;
    }
    else
    {
        result = (leftValue.toFloat() < rightValue.toFloat());
    }

    return result ? "true" : "false";
}

String Expression::evaluateGreater(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = false;
    if (isVector(*node->left) || isVector(*node->right))
    {
        os.console.error("Expression") << "Vector comparison is not supported yet." << os.console.endl;
    }
    else
    {
        result = (leftValue.toFloat() > rightValue.toFloat());
    }
    return result ? "true" : "false";
}

String Expression::evaluateLessEqual(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = false;
    if (isVector(*node->left) || isVector(*node->right))
    {
        os.console.error("Expression") << "Vector comparison is not supported yet." << os.console.endl;
    }
    else
    {
        result = (leftValue.toFloat() <= rightValue.toFloat());
    }
    return result ? "true" : "false";
}

String Expression::evaluateGreaterEqual(std::shared_ptr<Node> node)
{
    String leftValue = evaluateNode(node->left);
    String rightValue = evaluateNode(node->right);
    bool result = false;
    if (isVector(*node->left) || isVector(*node->right))
    {
        os.console.error("Expression") << "Vector comparison is not supported yet." << os.console.endl;
    }
    else
    {
        result = (leftValue.toFloat() >= rightValue.toFloat());
    }
    return result ? "true" : "false";
}

String Expression::evaluateVariable(std::shared_ptr<Node> node)
{
    String variableName = node->value;

    // Look up the value of the variable based on its name
    // This will depend on how variables are stored in your system
    String value = lookupVariableValue(variableName);

    return value;
}

String Expression::evaluateAssign(std::shared_ptr<Node> node)
{

    String value = evaluateNode(node->right);
    if(node->left->type == VARIABLE){
        Expression::setVariable(node->left->value, value);
    }
    return value;
}

String Expression::lookupVariableValue(const String &variableName)
{
    auto it = getVariables().find(variableName);
    if (it != getVariables().end())
    {
        return it->second; // Return the value of the variable
    }
    else
    {
        // Handle error: undefined variable
        os.console.error("Expression") << "Error: Undefined variable " + variableName << os.console.endl;
        return "error";
    }
}