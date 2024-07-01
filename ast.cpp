// definitions for classes in ast.h

#include "ast.h"

// returns the token literal
std::string Node::tokenLiteral(){
    return token.literal;
}

// default constructor for Program
Program::Program(){
    statements = std::vector<Statement*>();
}

// destructor for Program
Program::~Program(){
    for(Statement* stmt : statements){
        delete stmt;
    }
}

// Overriding tokenLiteral from Node
std::string Program::tokenLiteral() {
    if(statements.size() > 0)
        return statements[0]->tokenLiteral();
    else
        return "";
}

// Overriding toString from Node for printing
std::string Program::toString(){
    std::string result = "";
    for(Statement* stmt : statements){
        result += stmt->toString();
    }
    return result;
}

// default constructor 
LetStatement::LetStatement(){
     //TODO: implement this
}

// Setting the token constructor
LetStatement::LetStatement(Token token){
    this->token = token;
}

// Overriding statementNode from Statement
// EFFECTS: 
void LetStatement::statementNode() {
}

// Overriding toString from Node for printing
std::string LetStatement::toString(){
    std::string output = "";
    output += tokenLiteral() + " ";
    output += name->toString()+ " = ";
    if(expressionValue != nullptr){
        output += expressionValue->toString();
    }
    output += ";";
    return output;
}

// default constructor for Identifier
Identifier::Identifier(Token token, std::string value){
    this->token = token;
    this->value = value;
}

// Overriding expressionNode from Expression
void Identifier::expressionNode() {
}

// Overriding toString from Node for printing
std::string Identifier::toString() {
    return value;
}

// constructor with token
ReturnStatement::ReturnStatement(Token token){
    this->token = token;
}

// Overriding statement node from statement
void ReturnStatement::statementNode() {
}

// Overriding toString from Node for printing
std::string ReturnStatement::toString(){
    std::string output = "";
    output += tokenLiteral() + " ";
    if(expressionValue != nullptr){
        output += expressionValue->toString();
    }
    output += ";";
    return output; 
}

// default constructor
ExpressionStatement::ExpressionStatement(Token token){
    this->token = token;
}

// constructor with expression
ExpressionStatement::ExpressionStatement(Expression* expression){
    this->expressionValue = expression;

}


// Overriding statementNode from Statement
void ExpressionStatement::statementNode(){
}

// Overriding toString from Node for printing
std::string ExpressionStatement::toString() {
    if(expressionValue != nullptr){
        return expressionValue->toString();
    }
    else 
        return "";
}

// Token constructor
IntegerLiteral::IntegerLiteral(Token token){
    this->token = token;

}

// Overriding expressionNode from Expression
void IntegerLiteral::expressionNode(){
}

// Overriding toString from Node for printing
std::string IntegerLiteral::toString() {
    return token.literal;
}

// expression node override
void PrefixExpression::expressionNode(){
}

// toString override
std::string PrefixExpression::toString() {
    std::string output = "";
    output += "(" + op + " ";
    output += right->toString();
    output += ")";
    return output;
}

// default constructor
PrefixExpression::PrefixExpression(Token token, std::string op){
    this->token = token;
    this->op = op;
}

// prefix destructor
PrefixExpression::~PrefixExpression(){
    delete right;
}

// constructor with token, operator, and left expression
InfixExpression::InfixExpression(Token token, std::string op, Expression* left){
    this->token = token;
    this->op = op;
    this->left = left;
}

// infix destructor
InfixExpression::~InfixExpression(){
    delete left;
    delete right;
}

// expression node override
void InfixExpression::expressionNode(){
}

// toString override
std::string InfixExpression::toString(){
    std::string output = "";
    output += "(";
    output += left->toString();
    output += " " + op + " ";
    output += right->toString();
    output += ")";
    return output;
}