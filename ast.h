// Abstract Syntax Tree for Monkey language in C++

#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "lexer.h"

class Node {
    public: 
        virtual std::string tokenLiteral() = 0;

        virtual ~Node() = default;

        // EFFECTS:  returns the string representation of the node for debugging
        virtual std::string toString() = 0;
};

class Expression : public Node {
    public: 
        virtual std::string expressionNode() = 0;
};

class Statement : public Node {
    public: 
        virtual std::string statementNode() = 0;

        // vars
        Token token;
        Expression* expressionValue;
};



class Program : public Node {
    public: 
        // default constructor for Program
        Program();

        // destructor for Program
        ~Program();

        // Overriding tokenLiteral from Node
        std::string tokenLiteral() override;

        // Overriding toString from Node for printing
        std::string toString() override;

        std::vector<Statement*> statements;
};

class Identifier : public Expression {
    public:
    // default constructor for Identifier
    Identifier(Token token, std::string value);

    // Overriding tokenLiteral from Node
    // EFFECTS:  returns the token literal
    std::string tokenLiteral() override;

    // Overriding expressionNode from Expression
    std::string expressionNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    // vars
    Token token;
    std::string value;
};

class LetStatement : public Statement {
    public:
    // default constructor 
    LetStatement();

    // Setting the token constructor
    LetStatement(Token token);

    // Overriding tokenLiteral from Statement
    //EFFECTS:  prints out the token assosciated with this node
    std::string tokenLiteral() override;

    // Overriding statementNode from Statement
    // EFFECTS: 
    std::string statementNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    // vars
    //Token token; from statement
    Identifier* name;
    //Expression* value; from statement
};

class ReturnStatement : public Statement {
    public:
    //  constructor with token
    ReturnStatement(Token token);

    // Overriding statement node from statement
    std::string statementNode() override;

    // Overriding tokenLiteral from Statement
    std::string tokenLiteral() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    private:
    // Token token; from statement
    // Expression* returnValue;
};

class ExpressionStatement : public Statement {
    public:
    // default constructor
    ExpressionStatement(Token token);

    // constructor with expression
    ExpressionStatement(Expression* expression);

    // Overriding tokenLiteral from Statement
    std::string tokenLiteral() override;

    // Overriding statementNode from Statement
    std::string statementNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    private:
    // vars
    // Token token; from statement
    // Expression* expression;
};

class IntegerLiteral: public Expression{
    public:
    // Token constructor
    IntegerLiteral(Token token);

    // Overriding tokenLiteral from Node
    std::string tokenLiteral() override;

    // Overriding expressionNode from Expression
    std::string expressionNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    // vars
    // Token token; from statement
    Token token;
    int value;
};

class PrefixExpression: public Expression{
    public:
    // default constructor
    PrefixExpression(Token token, std::string op);

    // prefix destructor
    ~PrefixExpression();

    // expression node override
    std::string expressionNode() override;

    // token literal override
    std::string tokenLiteral() override;

    // toString override
    std::string toString() override;

    //vars
    Token token;
    std::string op;
    Expression* right;
};

#endif // AST_H