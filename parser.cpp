// implementations of parser.h

#include "parser.h"

// default constructor for Parser
Parser::Parser(Lexer* lexer){
    this->lexer = lexer;
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
    while(currentToken.type != TokenType::ENDOFFILE){
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
        default:
            return nullptr;
    }
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
    else
        return false;

}

// Checks if the current token is a certain type
bool Parser::curTokenIs(TokenType type){
    return currentToken.type == type;

}

// Checks if the peek token is a certain type
bool Parser::peekTokenIs(TokenType type){
    return peekToken.type == type;
}
