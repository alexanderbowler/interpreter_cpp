// definitions for classes in ast.h

#include "ast.h"

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

// Overriding tokenLiteral from Statement
//EFFECTS:  prints out the token assosciated with this node
std::string LetStatement::tokenLiteral() {
    return token.literal;
}

// Overriding statementNode from Statement
// EFFECTS: 
std::string LetStatement::statementNode() {
    return ""; //TODO: implement this
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

// Overriding tokenLiteral from Node
// EFFECTS:  returns the token literal
std::string Identifier::tokenLiteral() {
    return token.literal;
}

// Overriding expressionNode from Expression
std::string Identifier::expressionNode() {
    return ""; //TODO: implement this
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
std::string ReturnStatement::statementNode() {
    return ""; //TODO: implement this
}

// Overriding tokenLiteral from Statement
std::string ReturnStatement::tokenLiteral() {
    return token.literal;
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

// Overriding tokenLiteral from Statement
std::string ExpressionStatement::tokenLiteral() {
    return token.literal;
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
std::string ExpressionStatement::statementNode(){
    return ""; //TODO: implement this
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

// Overriding tokenLiteral from Node
std::string IntegerLiteral::tokenLiteral(){
    return token.literal;
}

// Overriding expressionNode from Expression
std::string IntegerLiteral::expressionNode(){
    return ""; //TODO: implement this

}

// Overriding toString from Node for printing
std::string IntegerLiteral::toString() {
    return token.literal;
}

// expression node override
std::string PrefixExpression::expressionNode(){
    return ""; //TODO: implement this
}

// token literal override
std::string PrefixExpression::tokenLiteral() {
    return token.literal;
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