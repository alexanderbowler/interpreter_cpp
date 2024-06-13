#include <gtest/gtest.h>
#include "lexer.h"
#include <string>

using namespace std;

struct ExpectedToken {
    TokenType expectedType;
    string expectedLiteral;
};

TEST(LexerTests, BasicTokenTest) {
    string input = "=+(){},;";
    ExpectedToken tests[9] = {
        {TokenType::ASSIGN, "="},
        {TokenType::PLUS, "+"},
        {TokenType::LPAREN, "("},
        {TokenType::RPAREN, ")"},
        {TokenType::LBRACE, "{"},
        {TokenType::RBRACE, "}"},
        {TokenType::COMMA, ","},
        {TokenType::SEMICOLON, ";"},
        {TokenType::ENDOFFILE, ""},
    };

    Lexer lexer = Lexer(input);
    for(ExpectedToken ex_tok : tests){
        Token tok = lexer.nextToken();
        EXPECT_EQ(tok.type, ex_tok.expectedType);
        EXPECT_EQ(tok.literal, ex_tok.expectedLiteral);
    }
}

TEST(LexerTests, BasicCodeTest) {
    string input = "let five = 5;\n \
let ten = 10;\n \
let add = fn(x, y) {\n \
x + y;\n \
};\n \
let result = add(five, ten);\n";
    ExpectedToken tests[37] = {
        {TokenType::LET, "let"},
        {TokenType::IDENT, "five"},
        {TokenType::ASSIGN, "="},
        {TokenType::INT, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENT, "ten"},
        {TokenType::ASSIGN, "="},
        {TokenType::INT, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENT, "add"},
        {TokenType::ASSIGN, "="},
        {TokenType::FUNCTION, "fn"},
        {TokenType::LPAREN, "("},
        {TokenType::IDENT, "x"},
        {TokenType::COMMA, ","},
        {TokenType::IDENT, "y"},
        {TokenType::RPAREN, ")"},
        {TokenType::LBRACE, "{"},
        {TokenType::IDENT, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENT, "y"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RBRACE, "}"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENT, "result"},
        {TokenType::ASSIGN, "="},
        {TokenType::IDENT, "add"},
        {TokenType::LPAREN, "("},
        {TokenType::IDENT, "five"},
        {TokenType::COMMA, ","},
        {TokenType::IDENT, "ten"},
        {TokenType::RPAREN, ")"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::ENDOFFILE, ""},
    };

    Lexer lexer = Lexer(input);
    for(ExpectedToken ex_tok : tests){
        Token tok = lexer.nextToken();
        EXPECT_EQ(tok.type, ex_tok.expectedType);
        EXPECT_EQ(tok.literal, ex_tok.expectedLiteral);
    }
}