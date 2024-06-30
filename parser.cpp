// implementations of parser.h

#include "parser.h"

// default constructor for Parser
Parser::Parser(Lexer* lexer){
    this->lexer = lexer;
    errors = std::vector<std::string>();
    // reads two tokens to initialize currentToken and peekToken
    nextToken();
    nextToken();
}

// Parses next token
void Parser::nextToken(){
    currentToken = peekToken;
    peekToken = lexer->nextToken();
}

Program* Parser::parseProgram(){
    Program* program = new Program();
    while(!curTokenIs(TokenType::ENDOFFILE)){
        Statement* stmt = parseStatement();
        if(stmt != nullptr){
            program->statements.push_back(stmt);
        }
        nextToken();
    }
    return program;
}

// Parses a statement and returns a statement pointer
Statement* Parser::parseStatement(){
    switch(currentToken.type){
        case TokenType::LET:
            return parseLetStatement();
        case TokenType::RETURN:
            return parseReturnStatement();
        default:
            return nullptr;
    }
}

// Parses a return statement and returns a statement pointer
Statement* Parser::parseReturnStatement(){
    ReturnStatement* stmt = new ReturnStatement(currentToken);
    nextToken();
    // TODO: implement logic for parsing expressions
    while(!curTokenIs(TokenType::SEMICOLON)){
        nextToken();
    }
    return stmt;
}

// Parses a let statement and returns a statement pointer
Statement* Parser::parseLetStatement(){
    LetStatement* stmt = new LetStatement(currentToken);
    if(!expectPeek(TokenType::IDENT)){ // increments the token if it is a identifier
        delete stmt;
        return nullptr; // error handling for if improper let statement
    }
    stmt->name = new Identifier(currentToken, currentToken.literal);
    if(!expectPeek(TokenType::ASSIGN)){
        delete stmt;
        return nullptr; // error handling for if improper let statement
    } 
    // TODO: skipping the experssions until get to semicolon
    while(!curTokenIs(TokenType::SEMICOLON)){
        nextToken();
    }
    return stmt;
}

// Checks if peek token is what we expect and advances the token if so
bool Parser::expectPeek(TokenType type){
    if(peekTokenIs(type)){
        nextToken();
        return true;
    }
    else{
        addPeekError(type);
        return false;

    }

}

// Checks if the current token is a certain type
bool Parser::curTokenIs(TokenType type){
    return currentToken.type == type;

}

// Checks if the peek token is a certain type
bool Parser::peekTokenIs(TokenType type){
    return peekToken.type == type;
}

// Adds a peek error to the errors vector
void Parser::addPeekError(TokenType type){
    std::string error = "expected next token to be " + lexer->TokenTypeToString[type]
    + " but got " + lexer->TokenTypeToString[peekToken.type] + " instead";
    errors.push_back(error);
}

// Returns the errors vector
std::vector<std::string>& Parser::getErrors(){
    return errors;
}