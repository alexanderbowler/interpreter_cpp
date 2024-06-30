// parser for monkeytype interpreter

#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "ast.h"
#include "lexer.h"

class Parser {
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

        // Checks if peek token is what we expect and advances the token if so
        bool expectPeek(TokenType type);

        // Checks if the current token is a certain type
        bool curTokenIs(TokenType type);

        // Checks if the peek token is a certain type
        bool peekTokenIs(TokenType type);

    private:
        Lexer* lexer;
        Token currentToken;
        Token peekToken;

};

#endif // PARSER_H