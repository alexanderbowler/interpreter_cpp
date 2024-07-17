#include "lexer.h"


// default lexer constructor
Lexer::Lexer(){
    input = "";
    position = 0;
    read_position = 0;
    ch = '\0';
};


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

    skipWhitespace();

    switch(ch){
        case '=':
            if(peekChar() == '='){
                readChar();
                tok = Token{TokenType::EQ, "=="};
            }
            else
                tok = Token{TokenType::ASSIGN, "="};
            break;
        case '+':
            tok = Token{TokenType::PLUS, "+"};
            break;
        case '-':
            tok = Token{TokenType::MINUS, "-"};
            break;
        case '!':
            if(peekChar() == '='){
                readChar();
                tok = Token{TokenType::NEQ, "!="};
            }
            else
                tok = Token{TokenType::BANG, "!"};
            break;
        case '*':
            tok = Token{TokenType::ASTERISK, "*"};
            break;
        case '/':
            tok = Token{TokenType::SLASH, "/"};
            break;
        case '<':
            tok = Token{TokenType::LT, "<"};
            break;
        case '>':
            tok = Token{TokenType::GT, ">"};
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
        case '\0':
            tok = Token{TokenType::ENDOFFILE, ""};
            break;
        case '\"':
            tok.type = TokenType::STRING;
            tok.literal = readString();
            break;
        case '[':
            tok = Token{TokenType::LBRACKET, "["};
            break;
        case ']':
            tok = Token{TokenType::RBRACKET, "]"};
            break;
        default:
            if(isalpha(ch) || ch == '_'){
                tok.literal = readIdentifier();
                tok.type = checkKeyword(tok.literal);
                return tok; // this is needed because readIdentifier uses readChar so we don't want to do it again
            }
            else if(isdigit(ch)){
                tok.literal = readDigit();
                tok.type = TokenType::INT;
                return tok; // this is needed because readIdentifier uses readChar so we don't want to do it again
            }
            else
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

// Peak at the next char
// EFFECTS:  returns the next character in the input
char Lexer::peekChar(){
    if(read_position >= input.length()){
        return 0;
    }
    else{
        return input[read_position];
    }
}

// Read an identifier
// EFFECTS:  reads an identifier string from the input
std::string Lexer::readIdentifier(){
    size_t initial_position = position;
    while(isalpha(ch) || ch == '_'){
        readChar();
    }
    return input.substr(initial_position, position - initial_position);
}

// Check type of string
// EFFECTS:  returns the type of the string if it is a keyword and identifier otherwise
TokenType Lexer::checkKeyword(std::string identifier){
    auto tokenIt = keywords.find(identifier);
    if(tokenIt != keywords.end())
        return tokenIt->second;
    else
        return TokenType::IDENT;
}

// Skips whitespace
// EFFECTS: skips the whitespace in input until ch is not whitespace
void Lexer::skipWhitespace(){
    while(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
        readChar();
}

// Read digits
// EFFECTS: reads in digits from the input
std::string Lexer::readDigit(){
    size_t initial_position = position;
    while(isdigit(ch))
        readChar();
    return input.substr(initial_position, position-initial_position);
}

// Reads a string as input and processes to put in as a literal
std::string Lexer::readString(){
    size_t beginIndex = position +1;
    while(true){
        readChar();
        if(ch == '\"' || ch == 0)
            break;
    }
    return input.substr(beginIndex, position-beginIndex);
}