// implementations of parser.h

#include "parser.h"

// default constructor for Parser
Parser::Parser(Lexer* lexer){
    this->lexer = lexer;
    errors = std::vector<std::string>();
    // reads two tokens to initialize currentToken and peekToken
    nextToken();
    nextToken();
    // register prefix parse functions
    registerPrefix(TokenType::IDENT, &Parser::parseIdentifier);
    registerPrefix(TokenType::INT, &Parser::parseIntegerLiteral);
    registerPrefix(TokenType::BANG, &Parser::parsePrefixExpression);
    registerPrefix(TokenType::MINUS, &Parser::parsePrefixExpression);
    registerInfix(TokenType::PLUS, &Parser::parseInfixExpression);
    registerInfix(TokenType::MINUS, &Parser::parseInfixExpression);
    registerInfix(TokenType::SLASH, &Parser::parseInfixExpression);
    registerInfix(TokenType::ASTERISK, &Parser::parseInfixExpression);
    registerInfix(TokenType::EQ, &Parser::parseInfixExpression);
    registerInfix(TokenType::NEQ, &Parser::parseInfixExpression);
    registerInfix(TokenType::GT, &Parser::parseInfixExpression);
    registerInfix(TokenType::LT, &Parser::parseInfixExpression);
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
            return parseExpressionStatement();
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

// Adds a prefix parse function to the prefixParseFns map
void Parser::registerPrefix(TokenType type, prefixParseFnPtr fn){
    prefixParseFns[type] = fn;
}

// Adds an infix parse function to the infixParseFns map
void Parser::registerInfix(TokenType type, infixParseFnPtr fn){
    infixParseFns[type] = fn;
}

// Parses an expression statement
ExpressionStatement* Parser::parseExpressionStatement(){
    ExpressionStatement* stmt = new ExpressionStatement(currentToken);
    stmt->expressionValue = parseExpression(LOWEST);
    if(peekTokenIs(TokenType::SEMICOLON)){
        nextToken();
    }
    return stmt;
}

// Parses an expression         
Expression* Parser::parseExpression(int precedence){
    prefixParseFnPtr prefixFn = prefixParseFns[currentToken.type];
    if(prefixFn == nullptr){
        noPrefixParseFnError(currentToken.type);
        return nullptr;
    }
    Expression* leftExp = (this->*prefixFn)();
    while(!peekTokenIs(TokenType::SEMICOLON) && precedence < peekPrecedence()){
        infixParseFnPtr infixFn = infixParseFns[peekToken.type];
        if(infixFn == nullptr){
            return leftExp;
        }
        nextToken();
        leftExp = (this->*infixFn)(leftExp);
    }
    return leftExp;
}

// Parses an identifier
Expression* Parser::parseIdentifier(){
    return new Identifier(currentToken, currentToken.literal);
}

// Parses an integer Literal
Expression* Parser::parseIntegerLiteral(){
    IntegerLiteral* lit = new IntegerLiteral(currentToken);
    try{
        int val = std::stoi(currentToken.literal);
        lit->value = val;
    }   
    catch(std::invalid_argument& e){
        std::string error = "could not parse " + currentToken.literal + " as integer";
        errors.push_back(error);
        return nullptr;
    }
    return lit;
}
    
// error catcher if there is no parser function
void Parser::noPrefixParseFnError(TokenType type){
    std::string message = "no prefix parse function for " + lexer->TokenTypeToString[type] + " found";
    errors.push_back(message);
}

// Parses a prefix expression
Expression* Parser::parsePrefixExpression(){
    PrefixExpression* expression = new PrefixExpression(currentToken, currentToken.literal);
    nextToken();
    expression->right = parseExpression(PREFIX);
    return expression;
}

// returns the precedence of the peeked token
int Parser::peekPrecedence(){
    int p = precedence[peekToken.type];
    if(p == 0)
        return LOWEST;
    return p;
}

// returns the precdeence of the current token
int Parser::curPrecedence(){
    int p = precedence[currentToken.type];
    if(p == 0)
        return LOWEST;
    return p;
}

// Parses an infix expression
Expression* Parser::parseInfixExpression(Expression* left){
    InfixExpression* expression = new InfixExpression(currentToken, currentToken.literal, left);
    int precedence = curPrecedence();
    nextToken();
    expression->right = parseExpression(precedence);
    return expression;
}