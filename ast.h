// Abstract Syntax Tree for Monkey language in C++

#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "lexer.h"

class Node {
    public: 
        virtual ~Node() = default;

        // EFFECTS:  returns the string representation of the node for debugging
        virtual std::string toString() = 0;

        // returns the token literal
        std::string tokenLiteral();

        //vars
        Token token;
};

class Expression : public Node {
    public: 
        virtual void expressionNode() = 0;
};

class Statement : public Node {
    public: 
        virtual void statementNode() = 0;

        // vars
        Expression* expressionValue;
};



class Program : public Node {
    public: 
        // default constructor for Program
        Program();

        // destructor for Program
        ~Program();

        // Overriding tokenLiteral from Node
        std::string tokenLiteral(); //overrides from Node

        // Overriding toString from Node for printing
        std::string toString() override;

        std::vector<Statement*> statements;
};

class Identifier : public Expression {
    public:
    // default constructor for Identifier
    Identifier(Token token, std::string value);

    // Overriding expressionNode from Expression
    void expressionNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    // vars
    std::string value;
};

class LetStatement : public Statement {
    public:
    // default constructor 
    LetStatement();

    // Setting the token constructor
    LetStatement(Token token);

    // Overriding statementNode from Statement
    // EFFECTS: 
    void statementNode() override;

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
    void statementNode() override;

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

    // Overriding statementNode from Statement
    void statementNode() override;

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

    // Overriding expressionNode from Expression
    void expressionNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    // vars
    // Token token; from statement
    int value;
};

class PrefixExpression: public Expression{
    public:
    // default constructor
    PrefixExpression(Token token, std::string op);

    // prefix destructor
    ~PrefixExpression();

    // expression node override
    void expressionNode() override;

    // toString override
    std::string toString() override;

    //vars
    std::string op;
    Expression* right;
};

class InfixExpression: public Expression{
    public:
    // constructor with token, operator, and left expression
    InfixExpression(Token token, std::string op, Expression* left);

    // infix destructor
    ~InfixExpression();

    // expression node override
    void expressionNode() override;

    // toString override
    std::string toString() override;

    //vars
    std::string op;
    Expression* left;
    Expression* right;
};

#endif // AST_H