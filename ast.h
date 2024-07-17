// Abstract Syntax Tree for Monkey language in C++

#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "lexer.h"

// Base class of nodes which the Abstract Syntax Tree is built on top of 
class Node {
    public: 
        virtual ~Node() = default;

        // EFFECTS:  returns the string representation of the node for debugging
        virtual std::string toString() = 0;

        // returns the token literal
        std::string tokenLiteral();

        //vars
        // all nodes have a token value which is the immediate token it represents, some nodes may 
        // have pointers to other nodes which will have their own token values
        Token token; // Tokens have a token type and a literal value ex. type: IDENT value: "x" or type: PLUS value: "+"
};

// base class for all of the expression nodes, used for arrays/pointers of expressions
// expressions are terms which can be evaluated to something example x+y or 5/5 or just x
class Expression : public Node { 
    public: 
        virtual void expressionNode() = 0;  // TODO: not used maybe for inhertiance might be able to get rid of
};

// base class for all of the statement nodes 
// statements full lines of code and have a large variety include assignment, returns, if/else, etc.
class Statement : public Node {
    public: 
        // destructor for Statement deletes the expression value
        ~Statement(){ 
            if(expressionValue)
                delete expressionValue;
        }

        virtual void statementNode() = 0; //TODO: not used might be needed for inhertiance may be able to get rid of

        // vars
        //Token token; from node
        Expression* expressionValue = nullptr; // Almost all statements need expression values so we include this here
                                    // expression values represent the expression attached to the statement for returns and lets
};


// Is the first node in the AST which holds all of the statements within a program
class Program : public Node {
    public: 
        // default constructor for Program
        Program();

        // destructor for Program, deletes all the statements
        ~Program();

        // Overriding tokenLiteral from Node
        // EFFECTS: returns the string token value for this node
        std::string tokenLiteral(); //overrides from Node

        // Overriding toString from Node for printing
        // EFFECTS: returns a string of all of the statements within the program, nicely formatted
        std::string toString() override;

        //vars
        // Token token; from node, not used for program
        // vector of the statements within a program
        std::vector<Statement*> statements;
};

// Expression node which holds an identifier as the token, value is the token literal,
// typically used in let statements 
// ex. let x = 1; identifier would have token of 'x' with value 'x'
class Identifier : public Expression {
    public:
    // default constructor for Identifier
    Identifier(Token token, std::string value);

    // Overriding expressionNode from Expression
    void expressionNode() override;

    // Overriding toString from Node for printing, just prints the identifier value ex. 'x'
    std::string toString() override;

    // vars
    // Token token; from node
    std::string value;
};

// Statement node which represents a let statement, has the token which is Let, identifier* name 
// which is the name of the var ex. 'x', and the expression* which is the value of the var ex. 5;
class LetStatement : public Statement {
    public:
    // default constructor 
    LetStatement();

    // destructor
    ~LetStatement(){ 
        if(name)
            delete name;
    } // don't need to delete value as thats taken care of by base class destructor

    // Setting the token constructor
    LetStatement(Token token);

    // Overriding statementNode from Statement
    void statementNode() override;

    // Overriding toString from Node for printing
    // EFFECTS: Prints the LetStatement is a legible way
    std::string toString() override;

    // vars
    //Token token; from node always LET
    Identifier* name = nullptr;
    //Expression* value; from statement
};

// Statement node which represents a return statement, has return token and expression* to value of return
class ReturnStatement : public Statement {
    public:
    //  constructor with token
    ReturnStatement(Token token);

    // no destructor required as taken care of by Statement

    // Overriding statement node from statement
    void statementNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    private:
    // Token token; from node, always RETURN
    // Expression* returnValue;
};

// Statement node which represents just a expression statement like 5+5;
// has only token and the expression* to its value;
class ExpressionStatement : public Statement {
    public:
    // default constructor
    ExpressionStatement(Token token);

    // no destructor required as taken care of by statement

    // constructor with expression
    ExpressionStatement(Expression* expression);

    // Overriding statementNode from Statement
    void statementNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    private:
    // vars
    // Token token; from node, token of first thing in expression statement
    // Expression* expression; // has the whole expression tree whether just value or complex
};

// Expression node of an integer literal represents a node in the tree of '5' for example.
class IntegerLiteral: public Expression{
    public:
    // Token constructor
    IntegerLiteral(Token token);

    // no destrucotr required taken care of by Expression

    // Overriding expressionNode from Expression
    void expressionNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    // vars
    // Token token; from node
    int value; // same as token.literal but as int 
};

// String Literal node represents the value of a string within an expression
class StringLiteral: public Expression{
    public:
    // Token constructor
    StringLiteral(Token token, std::string lit){
        this->token = token;
        value = lit;
    }

    // no destrucotr required taken care of by Expression

    // Overriding expressionNode from Expression
    void expressionNode() override;

    // Overriding toString from Node for printing
    std::string toString() override;

    //vars
    // Token token; from node
    std::string value;
};

class ArrayLiteral: public Expression{
    public:
        //constructor
        ArrayLiteral(Token tok){
            token = tok;
        }

        // destructor none needed as taken care of by vector

        // Overriding expressionNode from Expression
        void expressionNode() override;

        // Overriding toString from Node for printing
        std::string toString() override;

        //vars
        // Token token; from node
        std::vector<Expression*> elements; // elements of the array
};

// Expression Node which holds the parts of a prefix expression, holds the operator and the expression to the right
class PrefixExpression: public Expression{
    public:
    // default constructor
    PrefixExpression(Token token, std::string op);

    // prefix destructor, deletes right
    ~PrefixExpression();

    // expression node override
    void expressionNode() override;

    // toString override
    std::string toString() override;

    //vars
    // Token token from node
    std::string op;
    Expression* right = nullptr;
};

// Expression Node which holds an infix expression like 5+5, has the operator, and left and right expression pointers
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
    // Token token; from node
    std::string op;
    Expression* left = nullptr; // expression pointer to the left
    Expression* right = nullptr; // expression pointer to the right
};

// Expression node which holds a boolean expression, like integerLiteral but for bools, just holds bool value
class Boolean : public Expression{
    public:
    //constructor
    Boolean(Token token, bool value);

    // override expression node
    void expressionNode() override;

    // toString for pretty prints
    std::string toString() override;

    //vars
    // Token token; from node
    bool value;
};

// Statement node which holds a whole block statement, can be a bunch of statements held in array
class BlockStatement: public Statement{
    public:
    // constructor
    BlockStatement(Token token){this->token = token;};

    // destructor deletes all the Statement* within the block statement
    ~BlockStatement();

    // statment node override
    void statementNode(){};

    // turns Block statement into a string
    std::string toString();

    //vars
    std::vector<Statement*> statements;
};

// Expression Node which holds a full if expression including the condition, consequence, and alternative
class IfExpression: public Expression{
    public:
    // constructor
    IfExpression(Token inToken){token = inToken;};

    // destructor deletes all the pointers if they are not null
    ~IfExpression();

    // overrides expressionNode from expression
    void expressionNode() override;

    // prints out if as a string
    std::string toString() override;

    //vars
    Expression* condition = nullptr;
    BlockStatement* consequence = nullptr;
    BlockStatement* alternative = nullptr;
};

// Expression Node which holds a function including its token, parameters, and body
class FunctionLiteral: public Expression{
    public:
    // Token constructor
    FunctionLiteral(Token token){this->token = token;};

    // destructor
    ~FunctionLiteral(){
        if(body)
            delete body;
        for(Identifier* param:parameters)
            delete param;
    }

    // returns a string of the function literal properly formated 
    std::string toString();

    // override expressionNode
    void expressionNode(){};

    //vars
    // Token token; from node
    std::vector<Identifier*> parameters;
    BlockStatement* body = nullptr;
};

// Expression Node which holds the calling of a function occurs when we see '(' and preceded by a 
// identifier or function literal
class CallExpression: public Expression {
    public:
    // constructor for call Expression
    CallExpression(Token token, Expression* function){
        this->token = token;
        this->function = function;
    }

    // destructor
    ~CallExpression(){
        if(function)
            delete function;
        for(Expression* argument: arguments)
            delete argument;
    }
    
    // override expressionNode
    void expressionNode(){};

    // to string for printing 
    std::string toString();

    //vars 
    // token; from node, is '(' 
    Expression* function = nullptr; // expression * to preceding function
    std::vector<Expression*> arguments; // arguments to the function
};

class IndexExpression: public Expression{
     public:
    // constructor for call Expression
    IndexExpression(Token token, Expression* left){
        this->token = token;
        this->left = left;
    }

    // destructor
    ~IndexExpression(){
        if(left)
            delete left;
        if(index)
            delete index;
    }
    
    // override expressionNode
    void expressionNode(){};

    // to string for printing 
    std::string toString();

    //vars 
    // token; from node, is '(' 
    Expression* left = nullptr; // expression* left of [
    Expression* index = nullptr; // expression * right of [

};

#endif // AST_H