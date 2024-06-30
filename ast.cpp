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