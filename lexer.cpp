#include "lexer.h"

// Lexer constructor
// REQUIRES: input is a string
// EFFECTS:  creates a Lexer object
Lexer::Lexer(std::string input){
    this->input = input;
    position = 0;
    read_position = 1;
    ch = input[position];
}

// Get the next token
// EFFECTS:  returns the next token in the input
Token Lexer::nextToken(){
    Token tok;
    switch(ch){
        case '=':
            tok = Token{TokenType::ASSIGN, "="};
            break;
        case '+':
            tok = Token{TokenType::PLUS, "+"};
            break;
        case '(':
            tok = Token{TokenType::LPAREN, "("};
            break;
        case ')':
            tok = Token{TokenType::RPAREN, ")"};
            break;
        case '{':
            tok = Token{TokenType::LBRACE, "{"};
            break;
        case '}':
            tok = Token{TokenType::RBRACE, "}"};
            break;
        case ',':
            tok = Token{TokenType::COMMA, ","};
            break;
        case ';':
            tok = Token{TokenType::SEMICOLON, ";"};
            break;
        case 0:
            tok = Token{TokenType::ENDOFFILE, ""};
            break;
        default:
            tok = Token{TokenType::ILLEGAL, ""};
    }
    readChar();
    return tok;
}

// Read the next character
// MODIFIES: ch, position, read_position
// EFFECTS:  reads the next character in the input
void Lexer::readChar(){
    if(read_position >= input.length()){
        ch = 0;
    }
    else{
        ch = input[read_position];
    }
    position = read_position;
    read_position += 1;
}