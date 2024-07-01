// parser for monkeytype interpreter

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "ast.h"
#include "lexer.h"



class Parser {
    static const int    LOWEST = 1, 
                        EQUALS = 2, // ==
                        LESSGREATER = 3, // > or <
                        SUM = 4, // +
                        PRODUCT = 5, // *
                        PREFIX = 6, // -X or !X
                        CALL = 7; // myFunction(X)

    typedef Expression* (Parser::*prefixParseFnPtr)();
    typedef Expression* (Parser::*infixParseFnPtr)(Expression*);
    public:
    // default constructor for Parser
        Parser(Lexer* lexer);

        // Parses next token
        void nextToken();

        // Parses the whole program filling returning a program pointer filled with
        // statements
        Program* parseProgram();

        // Parses a statement and returns a statement pointer
        Statement* parseStatement();

        // Parses a let statement and returns a statement pointer
        Statement* parseLetStatement();

        // Parses a return statement and returns a statement pointer
        Statement* parseReturnStatement();

        // Checks if peek token is what we expect and advances the token if so
        bool expectPeek(TokenType type);

        // Checks if the current token is a certain type
        bool curTokenIs(TokenType type);

        // Checks if the peek token is a certain type
        bool peekTokenIs(TokenType type);

        // Adds a peek error to the errors vector
        void addPeekError(TokenType type);

        // Returns the errors vector
        std::vector<std::string>& getErrors();

        // Adds a prefix parse function to the prefixParseFns map
        void registerPrefix(TokenType type, prefixParseFnPtr fn);

        // Adds an infix parse function to the infixParseFns map
        void registerInfix(TokenType type, infixParseFnPtr fn);

        // Parses an expression statement
        ExpressionStatement* parseExpressionStatement();

        // Parses an expression
        Expression* parseExpression(int precedence);

        // Parses an identifier
        Expression* parseIdentifier();

        // Parses an integer Literal
        Expression* parseIntegerLiteral();

        // error catcher if there is no parser function
        void noPrefixParseFnError(TokenType type);

        // Parses a prefix expression
        Expression* parsePrefixExpression();

    private:
        Lexer* lexer;
        Token currentToken;
        Token peekToken;
        std::vector<std::string> errors;
        std::unordered_map<TokenType, prefixParseFnPtr> prefixParseFns;
        std::unordered_map<TokenType, infixParseFnPtr> infixParseFns;
};

#endif // PARSER_H