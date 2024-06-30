#include <gtest/gtest.h>
#include "lexer.h"
#include "parser.h"
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

TEST(LexerTests, SpecialSymbolsAndKeywordsTest) {
    string input = "let five = 5;\
let ten = 10;\
\
let add = fn(x, y) {\
  x + y;\
};\
\
let result = add(five, ten);\
!-/*5;\
5 < 10 > 5;\
\
if (5 < 10) { \
	return true; \
} else { \
	return false; \
}\
\
10 == 10;\
10 != 9;\
";
    ExpectedToken tests[74] = {
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
        {TokenType::BANG, "!"},
        {TokenType::MINUS, "-"},
        {TokenType::SLASH, "/"},
        {TokenType::ASTERISK, "*"},
        {TokenType::INT, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::INT, "5"},
        {TokenType::LT, "<"},
        {TokenType::INT, "10"},
        {TokenType::GT, ">"},
        {TokenType::INT, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::IF, "if"},
        {TokenType::LPAREN, "("},
        {TokenType::INT, "5"},
        {TokenType::LT, "<"},
        {TokenType::INT, "10"},
        {TokenType::RPAREN, ")"},
        {TokenType::LBRACE, "{"},
        {TokenType::RETURN, "return"},
        {TokenType::TRUE, "true"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RBRACE, "}"},
        {TokenType::ELSE, "else"},
        {TokenType::LBRACE, "{"},
        {TokenType::RETURN, "return"},
        {TokenType::FALSE, "false"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RBRACE, "}"},
        {TokenType::INT, "10"},
        {TokenType::EQ, "=="},
        {TokenType::INT, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::INT, "10"},
        {TokenType::NEQ, "!="},
        {TokenType::INT, "9"},
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

bool testLetStatement(Statement* s, string name);

TEST(ParserTests, LetStatementTest) {
    string input = "let x = 5;\
    let y = 10;\
    let foobar = 838383;";

    Lexer lexer = Lexer(input);
    Parser parser = Parser(&lexer);

    Program* program = parser.parseProgram();
    ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";

    ASSERT_EQ(program->statements.size(), 3) 
    << "ERROR MSG: Program.statements does not contain 3 statements got=" << program->statements.size();

    vector<string> expectedIdentifiers = {"x", "y", "foobar"};

    for(int i = 0; i < 3; i++){
        Statement* stmt = program->statements[i];
        if(testLetStatement(stmt, expectedIdentifiers[i]) == false){
            return;
        }
    }

}

bool testLetStatement(Statement* s, string name){
    if(s->tokenLiteral() != "let"){
        ADD_FAILURE() << "ERROR MSG: s.tokenLiteral not 'let'. got=" << s->tokenLiteral();
        return false;
    }
    // Attempt to dynamically cast Statement& to LetStatement&
    try {
        LetStatement& letStmt = dynamic_cast<LetStatement&>(*s);
        // If the cast succeeds, you can further test letStmt here
        // For example, checking the name matches the expected name
        if(letStmt.name->value != name){
            ADD_FAILURE() << "ERROR MSG: Expected name was not matched, got=" << letStmt.name->value;
            return false;
        }
        if(letStmt.name->tokenLiteral() != name){
            ADD_FAILURE() << "ERROR MSG: Expected statement.name->tokenLiteral was not matched, got=" << letStmt.name->token.literal;
            return false;
        }
    } catch (const std::bad_cast& e) {
        // If the dynamic cast fails, it means it is not a LetStatement
        ADD_FAILURE() << "ERROR MSG: Statement is not a LetStatement. Dynamic cast failed.";
        return false;
    }
    return true;
}