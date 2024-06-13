// Lexer for Monkey language in C++

#ifndef LEXER_H
#define LEXER_H

#include <string>

enum class TokenType {
    IDENT, // identifier for vars and function names
    ILLEGAL,
    ENDOFFILE,
    INT,
    ASSIGN,
    PLUS,
    COMMA,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    FUNCTION,
    LET,
};

struct Token {
    TokenType type;
    std::string literal;
};

class Lexer{
    public:
        // Lexer constructor
        // REQUIRES: input is a string
        // EFFECTS:  creates a Lexer object
        Lexer(std::string input);

        // Get the next token
        // EFFECTS:  returns the next token in the input
        Token nextToken();

    private:
        std::string input;
        int position; // current position in input
        int read_position; // current reading position in input (after current char)
        char ch; // current char under examination

        // Read the next character
        // MODIFIES: ch, position, read_position
        // EFFECTS:  reads the next character in the input
        void readChar();
        
};


#endif // LEXER_H