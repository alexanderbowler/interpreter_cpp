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
};

class Statement : public Node {
    public: 
        virtual std::string statementNode() = 0;
};

class Expression : public Node {
    public: 
        virtual std::string expressionNode() = 0;
};

class Program : public Node {
    public: 
        // default constructor for Program
        Program();

        // destructor for Program
        ~Program();

        // Overriding tokenLiteral from Node
        std::string tokenLiteral() override;

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

    // vars
    Token token;
    Identifier* name;
    Expression* value;
};


#endif // AST_H