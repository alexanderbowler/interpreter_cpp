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
     name = nullptr;
}

// Setting the token constructor
LetStatement::LetStatement(Token token){
    this->token = token;
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

// Overriding toString from Node for printing
std::string Identifier::toString() {
    return value;
}

// constructor with token
ReturnStatement::ReturnStatement(Token token){
    this->token = token;
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

// Overriding toString from Node for printing
std::string IntegerLiteral::toString() {
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
    if(right)
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
    if(left)
        delete left;
    if(right)
        delete right;
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

//constructor
Boolean::Boolean(Token token, bool value){
    this->token = token;
    this->value = value;
}

std::string Boolean::toString() {
    return token.literal;
}

// destructor
IfExpression::~IfExpression(){
    if(condition)
        delete condition;
    if(consequence)
        delete consequence;
    if(alternative)
        delete alternative;
}

// prints out if as a string
std::string IfExpression::toString() {
    std::string output = "if";
    output += condition->toString();
    output += " ";
    output += consequence->toString();
    if(alternative != nullptr){
        output += " else " + alternative->toString();
    }
    return output;
}

// destructor deletes all the Statement* within the block statement
BlockStatement::~BlockStatement(){
    for(Statement* stmt: statements){
        delete stmt;
    }
}

// turns Block statment into a string
std::string BlockStatement::toString(){
    std::string output = "";
    for(Statement* s : statements){
        output += s->toString();
    }
    return output;
}

// returns a string of the function literal properly formated 
std::string FunctionLiteral::toString(){
    std::string output = "";
    output += tokenLiteral();
    output += "(";
    for(size_t i = 0; i < parameters.size(); i++){
        output += parameters[i]->toString() ;
        if(i+1<parameters.size())
        output += ", ";
    }
    output += ")";
    output += body->toString();
    return output;
}

// to string for printing 
std::string CallExpression::toString(){
    std::string output = "";
    output += function->toString() + "(";
    for(size_t i = 0; i < arguments.size(); i++){
        output += arguments[i]->toString();
        if(i+1 < arguments.size())
            output += ", "; 
    }
    output += ")";
    return output;
}

// Overriding toString from Node for printing
std::string StringLiteral::toString() {
    return token.literal;
}

// Overriding toString from Node for printing
std::string ArrayLiteral::toString() {
    std::string output = "[";
    for(size_t i = 0; i < elements.size(); i++){
        output += elements[i]->toString();
        if(i+1 < elements.size())
            output += ", ";
    }
    output += "]";
    return output;
}

// to string for printing 
std::string IndexExpression::toString(){
    std::string output = "(";
    output += left->toString();
    output += "[";
    output += index->toString();
    output += "])";
    return output;
}

 // to string for printing 
std::string HashLiteral::toString(){
    std::string output = "{";
    for(auto it = pairs.begin(); it != pairs.end(); it++){
        output += it->first->toString() + ":" + it->second->toString();
        auto ahead = it;
        ahead++;
        if(ahead != pairs.end())
            output += ", ";
    }
    output += "}";
    return output;
}