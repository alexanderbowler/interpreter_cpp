// Lexer for Monkey language in C++

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>

enum class TokenType : uint8_t {
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
    MINUS,
    BANG,
    ASTERISK,
    SLASH,
    LT,
    GT,
    NEQ,
    EQ,
    FALSE,
    TRUE,
    RETURN,
    IF,
    ELSE,
    STRING,
    LBRACKET,
    RBRACKET,
    COLON,
};



struct Token {
    TokenType type;
    std::string literal;
};

class Lexer{
    public:
        std::unordered_map<TokenType, std::string> TokenTypeToString = {
            {TokenType::IDENT, "IDENT"},
            {TokenType::ILLEGAL, "ILLEGAL"},
            {TokenType::ENDOFFILE, "ENDOFFILE"},
            {TokenType::INT, "INT"},
            {TokenType::ASSIGN, "="},
            {TokenType::PLUS, "+"},
            {TokenType::COMMA, ","},
            {TokenType::SEMICOLON, ";"},
            {TokenType::LPAREN, "("},
            {TokenType::RPAREN, ")"},
            {TokenType::LBRACE, "{"},
            {TokenType::RBRACE, "}"},
            {TokenType::FUNCTION, "FUNCTION"},
            {TokenType::LET, "LET"},
            {TokenType::MINUS, "-"},
            {TokenType::BANG, "!"},
            {TokenType::ASTERISK, "*"},
            {TokenType::SLASH, "/"},
            {TokenType::LT, "<"},
            {TokenType::GT, ">"},
            {TokenType::NEQ, "!="},
            {TokenType::EQ, "=="},
            {TokenType::FALSE, "FALSE"},
            {TokenType::TRUE, "TRUE"},
            {TokenType::RETURN, "RETURN"},
            {TokenType::IF, "IF"},
            {TokenType::ELSE, "ELSE"},
            {TokenType::LBRACKET, "["},
            {TokenType::RBRACKET, "]"},
            {TokenType::COLON, ":"},
            };

        // Lexer constructor
        // REQUIRES: input is a string
        // EFFECTS:  creates a Lexer object
        Lexer(std::string input);

        // default lexer constructor
        Lexer();

        // Get the next token
        // EFFECTS:  returns the next token in the input
        Token nextToken();

    private:
        std::string input;
        size_t position; // current position in input
        size_t read_position; // current reading position in input (after current char)
        char ch; // current char under examination
        std::unordered_map<std::string, TokenType> keywords{ // map of the keywords in the language
            {"fn", TokenType::FUNCTION},
            {"let", TokenType::LET},
            {"true", TokenType::TRUE},
            {"false", TokenType::FALSE},
            {"return", TokenType::RETURN},
            {"if", TokenType::IF},
            {"else", TokenType::ELSE}
        };

        

        // Read the next character
        // MODIFIES: ch, position, read_position
        // EFFECTS:  reads the next character in the input
        void readChar();

        // Peak at the next char
        // EFFECTS:  returns the next character in the input
        char peekChar();

        // Read an identifier
        // EFFECTS:  reads an identifier string from the input
        std::string readIdentifier();

        // Check type of string
        // EFFECTS:  returns the type of the string if it is a keyword and identifier otherwise
        TokenType checkKeyword(std::string identifier);

        // Skips whitespace
        // EFFECTS: skips the whitespace in input until ch is not whitespace
        void skipWhitespace();

        // Read digits
        // EFFECTS: reads in digits from the input
        std::string readDigit();

        // Reads a string as input and processes to put in as a literal
        std::string readString();
        
};


#endif // LEXER_H