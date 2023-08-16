#pragma once
#include "system/core/lib.h"
#include "token.h"

struct Node {
    TokenType type;
    String value; // For literals and variables
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node() : type(TokenType::END_OF_EXPRESSION), value(""), left(nullptr), right(nullptr){};
    Node(TokenType t, String s, std::shared_ptr<Node> leftPtr, std::shared_ptr<Node> rightPtr) : type(t), value(s), left(leftPtr), right(rightPtr){};
};

enum VectorType {
    NOT_VECTOR,
    VEC2,
    VEC3
};

class Expression {
public:
    Expression(const String& input);
    String evaluate();
    
    static void registerVariables(const String& varname, const String& description);

    inline const String& toString(){return input;}
private:
    String input;
    size_t pos;
    Token currentToken; // Current token in the input string

    // Parsing functions
    std::shared_ptr<Node> parseExpression();
    std::shared_ptr<Node> parseFactor();
    std::shared_ptr<Node> parseTerm();
    std::shared_ptr<Node> parseLiteral();
    std::shared_ptr<Node> parseVariable();
    std::shared_ptr<Node> parseVector();

    // Evaluation functions
    String evaluateNode(std::shared_ptr<Node> node);
    String evaluateAdd(std::shared_ptr<Node> node);
    String evaluateSubtract(std::shared_ptr<Node> node);
    
    String evaluateMultiply(std::shared_ptr<Node> node);
    String evaluateDivide(std::shared_ptr<Node> node);
    String evaluateAnd(std::shared_ptr<Node> node);
    String evaluateOr(std::shared_ptr<Node> node);
    String evaluateEqual(std::shared_ptr<Node> node);
    String evaluateNotEqual(std::shared_ptr<Node> node);
    String evaluateNot(std::shared_ptr<Node> node);
    String evaluateLessEqual(std::shared_ptr<Node> node);
    String evaluateGreaterEqual(std::shared_ptr<Node> node);
    String evaluateLess(std::shared_ptr<Node> node);
    String evaluateGreater(std::shared_ptr<Node> node);
    String evaluateVariable(std::shared_ptr<Node> node); // Handle variables

    // Tokenization functions
    bool currentTokenIs(TokenType type);
    String currentTokenValue();
    void consumeToken();
    bool Expression::isVector(const String& value);
    VectorType getVectorType(const String& value);

    String addVectorsOrScalar(const String& left, const String& right);
    String subVectorsOrScalar(const String& left, const String& right);
    String multVectorsOrScalar(const String& left, const String& right);
    String divVectorsOrScalar(const String& left, const String& right);

    std::shared_ptr<Node> root;
    static std::map<String, String> variables; // A mapping from variable names to values
    static String lookupVariableValue(const String& variableName);
};
