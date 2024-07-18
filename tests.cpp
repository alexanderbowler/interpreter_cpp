#include <gtest/gtest.h>
#include "lexer.h"
#include "parser.h"
#include "object.h"
#include <string>
#include <variant>
#include "evaluator.h"
#include "environment.h"

using namespace std;

struct ExpectedToken {
    TokenType expectedType;
    string expectedLiteral;
};

//helper function definitions:
bool testLetStatement(Statement* s, string name);
void checkParserErrors(Parser& p);
bool testReturnStatement(Statement* s);
bool testIntegerLiteral(Expression* expression, int value);
template <typename leftType, typename rightType>
bool testInfixExpression(Expression* exp, leftType left, std::string op, rightType right);
bool testIdentifier(Expression* exp, std::string value);
bool testBooleanLiteral(Expression* exp, bool value);

//helper class
class TestLiteralExpression {
    Expression* exp;
    public:
    // Constructor
    TestLiteralExpression(Expression* exp): exp(exp) {}

    bool operator()(int value) const {
        return testIntegerLiteral(exp, value);
    }

    bool operator()(const std::string& value) const {
        return testIdentifier(exp, value);
    }
    bool operator()(const bool value) const {
        return testBooleanLiteral(exp, value);
    }
};

//// TESTS FOR THE LEXER:{
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
    \"foobar\"\
    \"foo bar\"\
    [1, 2];\
    {\"foo\": \"bar\"}\
    ";
    ExpectedToken tests[] = {
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
        {TokenType::STRING, "foobar"},
        {TokenType::STRING, "foo bar"},    
        {TokenType::LBRACKET, "["},
        {TokenType::INT, "1"},
        {TokenType::COMMA, ","},
        {TokenType::INT, "2"},
        {TokenType::RBRACKET, "]"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LBRACE, "{"},
        {TokenType::STRING, "foo"},
        {TokenType::COLON, ":"},
        {TokenType::STRING, "bar"},
        {TokenType::RBRACE, "}"}, 
        {TokenType::ENDOFFILE, ""},
    };

    Lexer lexer = Lexer(input);
    for(ExpectedToken ex_tok : tests){
        Token tok = lexer.nextToken();
        EXPECT_EQ(tok.type, ex_tok.expectedType);
        EXPECT_EQ(tok.literal, ex_tok.expectedLiteral);
    }
}


// PARSER TESTS:
TEST(ParserTests, LetStatementsTest) {
    struct {
        std::string input;
        std::string expectedIdentifier;
        std::variant<int, std::string, bool> expectedValue;
    } letTests[] = {
        {"let x = 5;", "x", 5},
        {"let y = true", "y", true},
        {"let foobar = y;", "foobar", "y"}
    };

    for(auto test: letTests){
        Lexer lexer = Lexer(test.input);
        Parser parser = Parser(&lexer);

        Program* program = parser.parseProgram();
        checkParserErrors(parser);
        ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";

        ASSERT_EQ(program->statements.size(), 1) 
        << "ERROR MSG: Program.statements does not contain 3 statements got=" << program->statements.size();
        
        if(!testLetStatement(program->statements[0], test.expectedIdentifier))
            return;

        if(!std::visit(TestLiteralExpression(program->statements[0]->expressionValue), test.expectedValue))
            return;
    }
}

TEST(ParserTests, ReturnStatementsTest) {
    struct {
        std::string input;
        std::variant<int, std::string, bool> expectedValue;
    } returnTests[] = {
        {"return 5;", 5},
        {"return true;", true},
        {"return foobar;", "foobar"}
    };
    for(auto test: returnTests){
        Lexer lexer = Lexer(test.input);
        Parser parser = Parser(&lexer);

        Program* program = parser.parseProgram();
        checkParserErrors(parser);
        ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";

        ASSERT_EQ(program->statements.size(), 1) 
        << "ERROR MSG: Program.statements does not contain 1 statement got=" << program->statements.size();

        if(!testReturnStatement(program->statements[0]))
            return;
        
        if(!std::visit(TestLiteralExpression(program->statements[0]->expressionValue), test.expectedValue))
            return;

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
            ADD_FAILURE() << "ERROR MSG: Expected statement.name->tokenLiteral was not matched, got=" << letStmt.name->tokenLiteral()<<" expected: "<<name;
            return false;
        }
    } catch (const std::bad_cast& e) {
        // If the dynamic cast fails, it means it is not a LetStatement
        ADD_FAILURE() << "ERROR MSG: Statement is not a LetStatement. Dynamic cast failed.";
        return false;
    }
    return true;
}

// parser helper functions
bool testReturnStatement(Statement* s){
    if(s->tokenLiteral() != "return"){
        ADD_FAILURE() << "ERROR MSG: s.tokenLiteral not 'return'. got=" << s->tokenLiteral();
        return false;
    }
    // Attempt to dynamically cast Statement& to ReturnStatement&
    try {
        ReturnStatement& returnStmt = dynamic_cast<ReturnStatement&>(*s);
        // If the cast succeeds, you can further test returnStmt here
        // For example, checking the name matches the expected name
    } catch (const std::bad_cast& e) {
        // If the dynamic cast fails, it means it is not a LetStatement
        ADD_FAILURE() << "ERROR MSG: Statement is not a ReturnStatement. Dynamic cast failed.";
        return false;
    }
    return true;
}

void checkParserErrors(Parser& p){
    vector<string> errors = p.getErrors();
    if(errors.size() == 0){
        return; // no errors
    }
    ADD_FAILURE() << "ERROR MSG: parser has " << errors.size() << " errors";
    for(string error : errors){
        ADD_FAILURE() << "parser error: " << error;
    }
}

// parser tests
TEST(ParserTests, TestStringLiteralExpression){
    std::string input = "\"hello world\";";
    Lexer lexer = Lexer(input);
    Parser parser = Parser(&lexer);
    Program* program = parser.parseProgram();
    checkParserErrors(parser);
    ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            StringLiteral* lit = dynamic_cast<StringLiteral*>(stmt->expressionValue);
            ASSERT_EQ(lit->value, "hello world") << "literal.value not \"hello world\". got="<<lit->value;
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "stmt->expressionValue not a string literal. Dynamic cast failed";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Program->stmt[0] not an expression statement. Dynamic cast failed";
    }
}

TEST(AstTest, ToStringTest){
    Program* program = new Program();
    LetStatement* letStmt = new LetStatement(Token{TokenType::LET, "let"});
    letStmt->name = new Identifier(Token{TokenType::IDENT, "myVar"}, "myVar");
    letStmt->expressionValue = new Identifier(Token{TokenType::IDENT, "anotherVar"}, "anotherVar");
    if(letStmt->toString() != "let myVar = anotherVar;"){
        ADD_FAILURE() << "ERROR MSG: letStmt.toString() wrong. got=" << letStmt->toString();
    }

}

TEST(ParserTests, IdentifierExpressionTest){
    string input = "foobar;";
    Lexer lexer = Lexer(input);
    Parser parser = Parser(&lexer);
    Program* program = parser.parseProgram();
    checkParserErrors(parser);
    ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";
    ASSERT_EQ(program->statements.size(), 1) << "ERROR MSG: Program.statements does not contain 1 statement got=" << program->statements.size();
    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            Identifier* ident = dynamic_cast<Identifier*>(stmt->expressionValue);
            EXPECT_EQ(ident->value, "foobar")<<"ident.value not 'foobar'. got="<<ident->value;
            EXPECT_EQ(ident->tokenLiteral(), "foobar")<<"ident.tokenLiteral not 'foobar'. got="<<ident->tokenLiteral();
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "ERROR MSG: expression not Identifier. Dynamic cast failed.";
            return;
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "ERROR MSG: program.statements[0] not ExpressionStatement. Dynamic cast failed.";
        return;
    }
}

TEST(ParserTests, IntegerLiteralExpressionTest){
    string input = "5;";
    Lexer lexer = Lexer(input);
    Parser parser = Parser(&lexer);
    Program* program = parser.parseProgram();
    checkParserErrors(parser);
    ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";
    ASSERT_EQ(program->statements.size(), 1) << "ERROR MSG: Program.statements does not contain 1 statement got=" << program->statements.size();
    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            IntegerLiteral* literal = dynamic_cast<IntegerLiteral*>(stmt->expressionValue);
            EXPECT_EQ(literal->value, 5)<<"ident.value not 5. got="<<literal->value;
            EXPECT_EQ(literal->tokenLiteral(), "5")<<"ident.tokenLiteral not '5'. got="<<literal->tokenLiteral();
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "ERROR MSG: expression not IntegerLiteral. Dynamic cast failed.";
            return;
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "ERROR MSG: program.statements[0] not ExpressionStatement. Dynamic cast failed.";
        return;
    }
}

TEST(ParserTests, PrefixParsingTest){
    struct {
        std::string input;
        std::string operation;
        int value;
    } prefixTests[] = {
        {"!5;", "!", 5},
        {"-15;", "-", 15},
    };
    for( int i = 0; i < 2; i++){
        Lexer lexer = Lexer(prefixTests[i].input);
        Parser parser = Parser(&lexer);
        Program* program = parser.parseProgram();
        checkParserErrors(parser);
        ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";
        ASSERT_EQ(program->statements.size(), 1) << "ERROR MSG: Program.statements does not contain 1 statement got=" << program->statements.size();
        try{
            ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
            try{
                PrefixExpression* expr = dynamic_cast<PrefixExpression*>(stmt->expressionValue);
                EXPECT_EQ(expr->op, prefixTests[i].operation)<<"ident.value not"<<prefixTests[i].operation<<". got="<<expr->op;
                if(!testIntegerLiteral(expr->right, prefixTests[i].value)){
                    return;
                }
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "ERROR MSG: expression not PrefixExpression. Dynamic cast failed.";
                return;
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "ERROR MSG: program.statements[0] not ExpressionStatement. Dynamic cast failed.";
            return;
        } 
    }
}

bool testIntegerLiteral(Expression* expression, int value){
    try{
        IntegerLiteral* literal = dynamic_cast<IntegerLiteral*>(expression);
        EXPECT_EQ(literal->value, value)<<"literal.value not "<<value<<". got="<<literal->value;
        EXPECT_EQ(literal->tokenLiteral(), std::to_string(value))<<"literal.tokenLiteral not "<<value<<". got="<<literal->tokenLiteral();
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "ERROR MSG: expression not IntegerLiteral. Dynamic cast failed.";
        return false;
    }
    return true;
}

TEST(ParserTests, InfixParsingTest){
    
    struct {
        std::string input;
        std::variant<int, std::string, bool> leftValue;
        std::string operation;
        std::variant<int, std::string, bool> rightValue;
    } infixTests[] = {
        {"5 + 5;",5,  "+", 5},
        {"5 - 5;", 5, "-", 5},
        {"5 * 5;", 5, "*", 5},
        {"5 / 5;", 5, "/", 5},
        {"5 > 5;", 5, ">", 5},
        {"5 < 5;", 5, "<", 5},
        {"5 == 5;", 5, "==", 5},
        {"5 != 5;", 5, "!=", 5},
        {"foobar + barfoo;", "foobar", "+", "barfoo"},
		{"foobar - barfoo;", "foobar", "-", "barfoo"},
		{"foobar * barfoo;", "foobar", "*", "barfoo"},
		{"foobar / barfoo;", "foobar", "/", "barfoo"},
		{"foobar > barfoo;", "foobar", ">", "barfoo"},
		{"foobar < barfoo;", "foobar", "<", "barfoo"},
		{"foobar == barfoo;", "foobar", "==", "barfoo"},
		{"foobar != barfoo;", "foobar", "!=", "barfoo"},
        {"true == true", true, "==", true},
		{"true != false", true, "!=", false},
		{"false == false", false, "==", false},
    };
    for( int i = 0; i < 19; i++){
        Lexer lexer = Lexer(infixTests[i].input);
        Parser parser = Parser(&lexer);
        Program* program = parser.parseProgram();
        checkParserErrors(parser);
        ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";
        ASSERT_EQ(program->statements.size(), 1) << "ERROR MSG: Program.statements does not contain 1 statement got=" << program->statements.size();
        try{
            ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
            try{
                InfixExpression* expr = dynamic_cast<InfixExpression*>(stmt->expressionValue);
                if(!testInfixExpression(expr, infixTests[i].leftValue, infixTests[i].operation, infixTests[i].rightValue))
                {
                    ADD_FAILURE() << "Test infix returned false";
                    return;
                }
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "ERROR MSG: expression not PrefixExpression. Dynamic cast failed.";
                return;
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "ERROR MSG: program.statements[0] not ExpressionStatement. Dynamic cast failed.";
            return;
        } 
    }
}

TEST(ParserTests, OperatorPrecedenceTest){
    struct {
        std::string input;
        std::string expected;
    } infixTests[] = {
        {"true;", "true"},
        {"false;", "false"},
        {"3 > 5 == false", "((3 > 5) == false)"},
        {"3 < 5 == true", "((3 < 5) == true)"},
        {"-a * b;",  "((- a) * b)"},
        {"!-a;", "(! (- a))"},
        {"a + b + c", "((a + b) + c)"},
        {"a + b - c", "((a + b) - c)"},
        {"a * b * c", "((a * b) * c)"},
        {"a * b / c", "((a * b) / c)"},
        {"a + b / c", "(a + (b / c))"},
        {"a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)"},
        {"3 + 4; -5 * 5", "(3 + 4)((- 5) * 5)"},
        {"5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))"},
        {"5 < 4 != 3 > 4", "((5 < 4) != (3 > 4))"},
        {"3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
        {
			"1 + (2 + 3) + 4",
			"((1 + (2 + 3)) + 4)",
		},
		{
			"(5 + 5) * 2",
			"((5 + 5) * 2)",
		},
		{
			"2 / (5 + 5)",
			"(2 / (5 + 5))",
		},
		{
			"(5 + 5) * 2 * (5 + 5)",
			"(((5 + 5) * 2) * (5 + 5))",
		},
		{
			"-(5 + 5)",
			"(- (5 + 5))",
		},
		{
			"!(true == true)",
			"(! (true == true))",
		},
        {
			"a + add(b * c) + d",
			"((a + add((b * c))) + d)",
		},
		{
			"add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))",
			"add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))",
		},
		{
			"add(a + b + c * d / f + g)",
			"add((((a + b) + ((c * d) / f)) + g))",
		},
        {
            "a * [1, 2, 3, 4][b * c] * d",
            "((a * ([1, 2, 3, 4][(b * c)])) * d)",
        },
        {
            "add(a * b[2], b[1], 2 * [1, 2][1])",
            "add((a * (b[2])), (b[1]), (2 * ([1, 2][1])))",
        },
    };
    for( int i = 0; i < 27; i++){
        Lexer lexer = Lexer(infixTests[i].input);
        Parser parser = Parser(&lexer);
        Program* program = parser.parseProgram();
        checkParserErrors(parser);
        std::string result = program->toString();
        EXPECT_EQ(result, infixTests[i].expected)<<"result not "<<infixTests[i].expected<<". got="<<result;
    }
}

bool testIdentifier(Expression* exp, std::string value){
    try{
        Identifier* ident = dynamic_cast<Identifier*>(exp);
        if(ident->value != value){
            ADD_FAILURE() << "ERROR MSG: ident.value not "<<value<<" got="<<ident->value;
            return false;
        }
        if(ident->tokenLiteral() != value){
            ADD_FAILURE() << "ERROR MSG: ident.tokenLiteral not "<<value<<" got="<<ident->tokenLiteral();
            return false;
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE()<<"ERROR MSG: expression not an identifier*. Failed dynamic cast";
        return false;
    }
    return true;
}

bool testBooleanLiteral(Expression* exp, bool value){
    try{
        Boolean* expBool = dynamic_cast<Boolean*>(exp);
        if(expBool->value != value){
            ADD_FAILURE() << "ERROR MSG: expBool.value not "<< value <<" got="<<expBool->value;
            return false;
        }
        if(expBool->tokenLiteral() != (value ? "true" : "false")){
            ADD_FAILURE() << "ERROR MSG: expBool->tokenLiteral() not "<<value<<" got="<<expBool->tokenLiteral();
            return false;
        }
        return true;
    }
    catch(const bad_cast& e){
        ADD_FAILURE() << "Failed to cast to Boolean* class";
        return false;
    }
}


// four overloaded functions
template <typename leftType, typename rightType>
bool testInfixExpression(Expression* exp, leftType left, std::string op, rightType right){
    try{ 
        InfixExpression* infixExp = dynamic_cast<InfixExpression*>(exp);
        if(infixExp->op != op){
            ADD_FAILURE() << "infixExpression.op is not "<< op << " got=" << infixExp->op;
            return false;
        }
        TestLiteralExpression leftVisitor(infixExp->left);
        TestLiteralExpression rightVisitor(infixExp->right);
        return std::visit(leftVisitor, left) && std::visit(rightVisitor,right);
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Expression not an infix expression. Dynamic cast failed";
        return false;
    }
    return true;
}

TEST(ParserTests, BooleanExpressionTests){
   struct {
    string input;
    bool expected;
   } inputs[] = {
    { "true;", true},
   {"false", false}
   };
    
    for(int i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++){

    
        Lexer lexer = Lexer(inputs[i].input);
        Parser parser = Parser(&lexer);

        Program* program = parser.parseProgram();
        checkParserErrors(parser);
        ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";

        ASSERT_EQ(program->statements.size(), 1) 
        << "ERROR MSG: Program.statements does not contain 1 statements got=" << program->statements.size();

        try {
            ExpressionStatement* exp = dynamic_cast<ExpressionStatement*>(program->statements[0]);
            try{
                Boolean* boolean = dynamic_cast<Boolean*>(exp->expressionValue);
                if(boolean->value != inputs[i].expected){
                    ADD_FAILURE() << "boolean.value not "<< inputs[i].expected<<" got="<<boolean->value;
                }
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "statement isn't Boolean class statement. Failed to dynamic cast";

            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "statement isn't expression statement. Failed to dynamic cast";
        }
    }
}

TEST(ParserTests, TestIfExpression){
     struct {
        std::string input;
        std::variant<int, std::string, bool> leftValue;
        std::string operation;
        std::variant<int, std::string, bool> rightValue;
        std::string consequence;
    } ifTests[] = {
        {"if (x < y) {x}", "x",  "<", "y", "x"},
    };
    Lexer l = Lexer(ifTests[0].input);
    Parser parser = Parser(&l);
    Program* program = parser.parseProgram();
    checkParserErrors(parser);
    ASSERT_EQ(program->statements.size(), 1) 
    << "ERROR MSG: Program.statements does not contain 1 statements got=" << program->statements.size();
    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            IfExpression* exp = dynamic_cast<IfExpression*>(stmt->expressionValue);
            if(!testInfixExpression(exp->condition, ifTests[0].leftValue, ifTests[0].operation, ifTests[0].rightValue))
                return;
            ASSERT_EQ(exp->consequence->statements.size(), 1) << "Consequences is not 1 statement. got="<< exp->consequence->statements.size()<<"\n";
            try{
                ExpressionStatement* consequence = dynamic_cast<ExpressionStatement*>(exp->consequence->statements[0]);
                if(!testIdentifier(consequence->expressionValue, ifTests[0].consequence))
                    return;
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "consequence.statments[0] is not ExpressionStatement. Dynamic Cast Failed.\n";
            }
            if(exp->alternative != nullptr)
                ADD_FAILURE() << "ifexpression.alternative is not nullptr.";
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "stmt.exp is not a IfExpression. Dynamic cast failed\n";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program.statement[0] is not ExpressionStatement. Dynamic cast failed\n";
    }
}

TEST(ParserTests, TestIfElseExpression){
    struct {
        std::string input;
        std::variant<int, std::string, bool> leftValue;
        std::string operation;
        std::variant<int, std::string, bool> rightValue;
        std::string consequence;
        std::string alternative;
    } ifTests[] = {
        {"if (x < y) {x} else {y}", "x",  "<", "y", "x", "y"},
    };
    Lexer l = Lexer(ifTests[0].input);
    Parser parser = Parser(&l);
    Program* program = parser.parseProgram();
    checkParserErrors(parser);
    ASSERT_EQ(program->statements.size(), 1) 
    << "ERROR MSG: Program.statements does not contain 1 statements got=" << program->statements.size();
    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            IfExpression* exp = dynamic_cast<IfExpression*>(stmt->expressionValue);
            if(!testInfixExpression(exp->condition, ifTests[0].leftValue, ifTests[0].operation, ifTests[0].rightValue))
                return;
            ASSERT_EQ(exp->consequence->statements.size(), 1) << "Consequences is not 1 statement. got="<< exp->consequence->statements.size()<<"\n";
            try{
                ExpressionStatement* consequence = dynamic_cast<ExpressionStatement*>(exp->consequence->statements[0]);
                if(!testIdentifier(consequence->expressionValue, ifTests[0].consequence))
                    return;
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "consequence.statments[0] is not ExpressionStatement. Dynamic Cast Failed.\n";
            }
            try{
                ExpressionStatement* alternative = dynamic_cast<ExpressionStatement*>(exp->alternative->statements[0]);
                if(!testIdentifier(alternative->expressionValue, ifTests[0].alternative))
                    return;
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "alternative.statements[0] is not an ExpressionStatement. Dynamic Cast Failed.\n";
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "stmt.exp is not a IfExpression. Dynamic cast failed\n";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program.statement[0] is not ExpressionStatement. Dynamic cast failed\n";
    }
}

TEST(ParserTests, TestFunctionLiteralParsing){
    struct {
        std::string input;
        std::vector<std::string> parameters;
        std::variant<int, std::string, bool> left;
        std::string operation;
        std::variant<int, std::string, bool> right;
    } fnTests[] = {
        {"fn(x, y) {x + y;}", std::vector<std::string>{"x", "y"}, "x", "+", "y"},
    };
    Lexer* l = new Lexer(fnTests[0].input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_EQ(program->statements.size(), 1) 
    << "ERROR MSG: Program.statements does not contain 1 statements got=" << program->statements.size();

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            FunctionLiteral* exp = dynamic_cast<FunctionLiteral*>(stmt->expressionValue);

            ASSERT_EQ(exp->parameters.size(), 2) << "FunctionLiteral.parameters is not 2 parameters. got="<< exp->parameters.size()<<"\n";

            for(size_t i = 0; i < fnTests[0].parameters.size(); i++){
                if(!testIdentifier(exp->parameters[i], fnTests[0].parameters[i]))
                    return;
            }

            ASSERT_EQ(exp->body->statements.size(), 1) << "function.body.statements not size 1. got="<<exp->body->statements.size();

            try{
                ExpressionStatement* bodyStmt = dynamic_cast<ExpressionStatement*>(exp->body->statements[0]);
                if(!testInfixExpression(bodyStmt->expressionValue, fnTests[0].left, fnTests[0].operation, fnTests[0].right))
                    return;
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "exp->body->statements is not expressionStatement. Dynamic Cast Failed.\n";
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "stmt.exp is not a FunctionLiteral. Dynamic cast failed\n";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program.statement[0] is not ExpressionStatement. Dynamic cast failed\n";
    }
}
TEST(ParserTests, TestFunctionParameterParsing){
    struct {
        std::string input;
        std::vector<std::string> parameters;
    } fnTests[] = {
        {"fn() {};", std::vector<std::string>{}},
        {"fn(x) {};", std::vector<std::string>{"x"}},
        {"fn(x, y) {};", std::vector<std::string>{"x", "y"}},
    };

    for(auto fnTest:fnTests){
        Lexer* l = new Lexer(fnTest.input);
        Parser p = Parser(l);
        Program* program = p.parseProgram();
        checkParserErrors(p);

        ASSERT_EQ(program->statements.size(), 1) 
        << "ERROR MSG: Program.statements does not contain 1 statements got=" << program->statements.size();

        try{
            ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
            try{
                FunctionLiteral* exp = dynamic_cast<FunctionLiteral*>(stmt->expressionValue);

                ASSERT_EQ(exp->parameters.size(), fnTest.parameters.size()) << "FunctionLiteral.parameters is not "<< fnTest.parameters.size()<<" parameters. got="<< exp->parameters.size()<<"\n";

                for(size_t i = 0; i < fnTest.parameters.size(); i++){
                    if(!testIdentifier(exp->parameters[i], fnTest.parameters[i]))
                        return;
                }
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "stmt.exp is not a FunctionLiteral. Dynamic cast failed\n";
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "program.statement[0] is not ExpressionStatement. Dynamic cast failed\n";
        }
    
    }
}

TEST(ParserTests, TestCallExpressionParsing){
    std::string input = "add(1, 2*3, 4+5)";
   Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_EQ(program->statements.size(), 1) 
    << "ERROR MSG: Program.statements does not contain 1 statements got=" << program->statements.size();

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            CallExpression* exp = dynamic_cast<CallExpression*>(stmt->expressionValue);

            if(!testIdentifier(exp->function, "add"))
                return;
            
            ASSERT_EQ(exp->arguments.size(), 3) << "Expected CallExpression->arguments.size() to be 3. got=" << exp->arguments.size();

            TestLiteralExpression(exp->arguments[0])(1);
            std::variant<int, std::string, bool> a = 2, b = 3, c = 4, d = 5;
            testInfixExpression(exp->arguments[1], a, "*", b);
            testInfixExpression(exp->arguments[2], c, "+", d);

        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "stmt.exp is not a CallExpression. Dynamic cast failed\n";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program.statement[0] is not ExpressionStatement. Dynamic cast failed\n";
    }
}

TEST(ParserTests, TestCallExpressionParameterParsing){
    struct {
        std::string input;
        std::string expectedIdent;
        std::vector<std::string> expectedArgs;
    } callExpressionTests[] = {
        {"add();", "add", std::vector<std::string>{}},
        {"add(1);", "add", std::vector<std::string>{"1"}},
        {"add(1, 2 * 3, 4 + 5);", "add", std::vector<std::string>{"1", "(2 * 3)", "(4 + 5)"}},
    };

	for(auto test:callExpressionTests) {
		Lexer* l = new Lexer(test.input);
		Parser p = Parser(l);
		Program* program = p.parseProgram();
		checkParserErrors(p);

		try{
            ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
            try{
                CallExpression* exp = dynamic_cast<CallExpression*>(stmt->expressionValue);

                if(!testIdentifier(exp->function, test.expectedIdent))
                    return;
                
                ASSERT_EQ(exp->arguments.size(), test.expectedArgs.size()) << "Expected CallExpression->arguments.size() to be" <<test.expectedArgs.size()<<". got=" << exp->arguments.size();

                for(size_t i = 0; i < test.expectedArgs.size(); i++){
                    EXPECT_EQ(exp->arguments[i]->toString(), test.expectedArgs[i]) << "Expected: "<<test.expectedArgs[i]<<" got: " << exp->arguments[i]->toString();
                }

            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "stmt.exp is not a CallExpression. Dynamic cast failed\n";
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "program.statement[0] is not ExpressionStatement. Dynamic cast failed\n";
        }
    }
}

TEST(ParserTests, TestArrayLiteral){
    std::string input = "[1, 2 * 2, 3 + 3]";
    Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            ArrayLiteral* array = dynamic_cast<ArrayLiteral*>(stmt->expressionValue);
            ASSERT_EQ(array->elements.size(), 3) << "array.elements.size() is not 3. got="<<array->elements.size();
            testIntegerLiteral(array->elements[0], 1);
            std::variant<std::string, int, bool> left = 2;
            std::variant<std::string, int, bool> right = 2;
            testInfixExpression(array->elements[1], left, "*", right);
            left = 3;
            right = 3;
            testInfixExpression(array->elements[2], left, "+", right);
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "Expression is not a ArrayLiteral*. Dynamic cast failed.";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program->stmts[0] not ExpressionStatement*. Dynamic cast failed.";
    }
}

TEST(ParserTests, TestArrayIndexing){
    std::string input = "myArray[1 + 1];";
    Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            IndexExpression* indexExp = dynamic_cast<IndexExpression*>(stmt->expressionValue);
            ASSERT_NE(indexExp->left, nullptr);
            if(!testIdentifier(indexExp->left, "myArray"))
                return;
            std::variant<std::string, int, bool> val = 1;
            if(!testInfixExpression(indexExp->index, val, "+", val))
                return;
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "Expression is not a IndexExpression*. Dynamic cast failed.";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program->stmts[0] not ExpressionStatement*. Dynamic cast failed.";
    }
}

TEST(ParserTests, TestHashLiteralsStringKeys){
     std::string input = "{\"one\":1, \"two\":2, \"three\":3};";
    Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            HashLiteral* hashLit = dynamic_cast<HashLiteral*>(stmt->expressionValue);
            ASSERT_EQ(hashLit->pairs.size(), 3) << "Hash literal does not contain 3 pairs. got=" << hashLit->pairs.size();
            std::unordered_map<std::string, int> expected = {
                {"one", 1},
                {"two", 2},
                {"three", 3}
            };
            for(auto hashIt = hashLit->pairs.begin(); hashIt != hashLit->pairs.end(); hashIt++){
                try{
                    StringLiteral* str = dynamic_cast<StringLiteral*>(hashIt->first);
                    int expectedValue = expected[str->toString()];
                    testIntegerLiteral(hashIt->second, expectedValue);
                }
                catch(const std::bad_cast& e){
                    ADD_FAILURE() << "key is not StringLiteral*. Dynamic cast failed.";
                }
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "Expression is not a HashLiteral*. Dynamic cast failed.";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program->stmts[0] not ExpressionStatement*. Dynamic cast failed.";
    }
}
TEST(ParserTests, TestHashLiteralsWithExpressions){
     std::string input = "{\"one\": 0+1, \"two\": 10 - 8, \"three\": 15 / 5};";
    Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            HashLiteral* hashLit = dynamic_cast<HashLiteral*>(stmt->expressionValue);
            ASSERT_EQ(hashLit->pairs.size(), 3) << "Hash literal does not contain 3 pairs. got=" << hashLit->pairs.size();
            std::unordered_map<std::string, 
                std::tuple<
                    std::variant<std::string, bool, int>, std::string, std::variant<std::string, bool, int>
                >
            > expected = {
                {"one", {0, "+", 1}},
                {"two", {10, "-", 8}},
                {"three", {15, "/", 5}}
            };
            for(auto hashIt = hashLit->pairs.begin(); hashIt != hashLit->pairs.end(); hashIt++){
                try{
                    StringLiteral* str = dynamic_cast<StringLiteral*>(hashIt->first);
                    auto expectedValue = expected[str->toString()];
                    testInfixExpression(hashIt->second, std::get<0>(expectedValue), std::get<1>(expectedValue), std::get<2>(expectedValue));
                }
                catch(const std::bad_cast& e){
                    ADD_FAILURE() << "key is not StringLiteral*. Dynamic cast failed.";
                }
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "Expression is not a HashLiteral*. Dynamic cast failed.";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program->stmts[0] not ExpressionStatement*. Dynamic cast failed.";
    }
}

TEST(ParserTests, TestHashLiteralsBooleanKeys){
     std::string input = "{true:1, false:2};";
    Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            HashLiteral* hashLit = dynamic_cast<HashLiteral*>(stmt->expressionValue);
            ASSERT_EQ(hashLit->pairs.size(), 2) << "Hash literal does not contain 2 pairs. got=" << hashLit->pairs.size();
            std::unordered_map<bool, int> expected = {
                {true, 1},
                {false, 2},
            };
            for(auto hashIt = hashLit->pairs.begin(); hashIt != hashLit->pairs.end(); hashIt++){
                try{
                    Boolean* str = dynamic_cast<Boolean*>(hashIt->first);
                    int expectedValue = expected[str->value];
                    testIntegerLiteral(hashIt->second, expectedValue);
                }
                catch(const std::bad_cast& e){
                    ADD_FAILURE() << "key is not Boolean*. Dynamic cast failed.";
                }
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "Expression is not a HashLiteral*. Dynamic cast failed.";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program->stmts[0] not ExpressionStatement*. Dynamic cast failed.";
    }
}

TEST(ParserTests, TestHashLiteralsIntegerKeys){
     std::string input = "{1:2, 2:3};";
    Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            HashLiteral* hashLit = dynamic_cast<HashLiteral*>(stmt->expressionValue);
            ASSERT_EQ(hashLit->pairs.size(), 2) << "Hash literal does not contain 2 pairs. got=" << hashLit->pairs.size();
            std::unordered_map<int, int> expected = {
                {1, 2},
                {2, 3},
            };
            for(auto hashIt = hashLit->pairs.begin(); hashIt != hashLit->pairs.end(); hashIt++){
                try{
                    IntegerLiteral* intLit = dynamic_cast<IntegerLiteral*>(hashIt->first);
                    int expectedValue = expected[intLit->value];
                    testIntegerLiteral(hashIt->second, expectedValue);
                }
                catch(const std::bad_cast& e){
                    ADD_FAILURE() << "key is not IntegerLiteral*. Dynamic cast failed.";
                }
            }
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "Expression is not a HashLiteral*. Dynamic cast failed.";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program->stmts[0] not ExpressionStatement*. Dynamic cast failed.";
    }
}

TEST(ParserTests, TestEmptyHashLiteral){
     std::string input = "{};";
    Lexer* l = new Lexer(input);
    Parser p = Parser(l);
    Program* program = p.parseProgram();
    checkParserErrors(p);

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    try{
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
        try{
            HashLiteral* hashLit = dynamic_cast<HashLiteral*>(stmt->expressionValue);
            ASSERT_EQ(hashLit->pairs.size(), 0) << "Hash literal does not contain 0 pairs. got=" << hashLit->pairs.size();
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "Expression is not a HashLiteral*. Dynamic cast failed.";
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "program->stmts[0] not ExpressionStatement*. Dynamic cast failed.";
    }
}

// Evaluator helper functions:
Object* testEval(std::string& input){
    Lexer l = Lexer(input);
    Parser p = Parser(&l);
    Program* program = p.parseProgram();
    Environment env = Environment();
    return Eval(program, &env);
}
bool testIntegerObject(Object* obj, int expectedVal){
    if(!obj)
        ADD_FAILURE() << "Object* is nullptr";
    try{
        Integer* obj_int = dynamic_cast<Integer*>(obj);
        if(obj_int->value != expectedVal){
            ADD_FAILURE() << "Expected Integer->value to be "<<expectedVal<<". got="<<obj_int->value;
            return false;
        }
        if(obj_int)
            delete obj_int;
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Object* is not Integer*. Dynamic cast failed";
        return false;
    }
    return true;
}

bool testBooleanObject(Object* obj, bool expectedVal){
    if(!obj)
        ADD_FAILURE() << "Object* is nullptr";
    try{
        BooleanObj* bool_obj = dynamic_cast<BooleanObj*>(obj);
        if(bool_obj->value != expectedVal){
            ADD_FAILURE() <<"Expected BooleanObj->value to be "<<expectedVal<<". got="<<bool_obj->value;
            return false;
        }
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Object* is not BooleanObj*. Dynamic cast failed";
        return false;
    }
    return true;
}

bool testNullObject(Object* obj){
    try{
        Null* nullObj = dynamic_cast<Null*>(obj);
        Null* nulltrue = &NULLOBJ;
        std::cout<<obj->inspect()<<NULLOBJ.inspect();

        if(obj->inspect() != NULLOBJ.inspect()){
            ADD_FAILURE() <<"object is not NULL";
            return false;
        }
        return true;
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() <<"object is not NULL";
        return false;
    }
    
    return true;
}


// EVALUATOR TESTS:
TEST(EvaluatorTests, TestIntegerExpression){
    struct {
        std::string input;
        int expectedValue;
    } tests[] = {
        {"5", 5},
        {"10", 10},
        {"-5", -5},
        {"-10", -10},
        {"5 + 5 + 5 + 5 - 10", 10},
        {"2 * 2 * 2 * 2 * 2", 32},
        {"-50 + 100 + -50", 0},
        {"5 * 2 + 10", 20},
        {"5 + 2 * 10", 25},
        {"20 + 2 * -10", 0},
        {"50 / 2 * 2 + 10", 60},
        {"2 * (5 + 10)", 30},
        {"3 * 3 * 3 + 10", 37},
        {"3 * (3 * 3) + 10", 37},
        {"(5 + 10 * 2 + 15 / 3) * 2 + -10", 50},
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        testIntegerObject(evaluated, test.expectedValue);
    }
}

TEST(EvaluatorTests, TestBooleanExpression){
    struct {
        std::string input;
        bool expectedValue;
    } tests[] = {
        {"true", true},
        {"false", false},
        {"1 < 2", true},
        {"1 > 2", false},
        {"1 < 1", false},
        {"1 > 1", false},
        {"1 == 1", true},
        {"1 != 1", false},
        {"1 == 2", false},
        {"1 != 2", true},
        {"true == true", true},
        {"false == false", true},
        {"true == false", false},
        {"true != false", true},
        {"false != true", true},
        {"(1 < 2) == true", true},
        {"(1 < 2) == false", false},
        {"(1 > 2) == true", false},
        {"(1 > 2) == false", true},
        {"\"hello\" == \"hello\"", true},
        {"\"hello\" == \"world\"", false},
        {"\"hello\" != \"hello\"", false},
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        testBooleanObject(evaluated, test.expectedValue);
    }
}

TEST(EvaluatorTests, TestBangOperator){
    struct {
        std::string input;
        bool expectedValue;
    } tests[] = {
        {"!true", false},
        {"!false", true},
        {"!5", false},
        {"!!true", true},
        {"!!false", false},
        {"!!5", true},
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        testBooleanObject(evaluated, test.expectedValue);
    }
}

TEST(EvaluatorTests, TestIfElseExpression){
    struct {
        std::string input;
        int expectedValue;
    } tests[] = {
        {"if (true) { 10 }", 10},
        {"if (false) { 10 }", -1}, // -1 is a just a flag for nullptr in these test cases
       {"if (1) { 10 }", 10},
       {"if (1 < 2) { 10 }", 10},
       {"if (1 > 2) { 10 }", -1},
       {"if (1 > 2) { 10 } else { 20 }", 20},
       {"if (1 < 2) { 10 } else { 20 }", 10},
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        
        if(test.expectedValue == -1){
            testNullObject(evaluated);
        }
        else{
            try{
                Integer* eval_int = dynamic_cast<Integer*>(evaluated);
                EXPECT_EQ(test.expectedValue, eval_int->value) << "IfElse value not "<<test.expectedValue<<". got=" << eval_int->value;
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() << "Object* is not an Integer*. Dynamic Cast failed.";
            }
        }
    }
}

TEST(EvaluatorTests, TestReturnStatements){
    struct {
        std::string input;
        int expectedValue;
    } tests[] = {
        {"return 10;", 10},
        {"return 10; 9;", 10},
        {"return 2 * 5; 9;", 10},
        {"9; return 2 * 5; 9;", 10},
        {"if (10 > 1){\
        if(10>1){\
        return 10;}\
        return 1;}", 10},
        
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        testIntegerObject(evaluated, test.expectedValue);
    }
}

TEST(EvaluatorTests, TestErrorHandling){
    struct {
        std::string input;
        std::string expectedMessage;
    } tests[] = {
        {
            "5 + true;",
            "type mismatch: INTEGER + BOOLEAN",
        },
        {
            "5 + true; 5;",
            "type mismatch: INTEGER + BOOLEAN",
        },
        {
            "-true",
            "unknown operator: -BOOLEAN",
        },
        {
            "true + false;",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            "5; true + false; 5",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            "if (10 > 1) { true + false; }",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            "\
            if (10 > 1) {\
            if (10 > 1) {\
                return true + false;\
            }\
            \
            return 1;\
            }\
            ",
            "unknown operator: BOOLEAN + BOOLEAN",
        },
        {
            "foobar",
            "identifier not found: foobar"
        },
        {
            "\"Hello\" - \"world\"",
            "unknown operator: STRING - STRING"
        },
        {
            "{\"name\": \"Monkey\"}[fn(x) { x }];",
            "unusable as hash key: FUNCTION",
        }
    };
        
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        try{
            Error* errorObj = dynamic_cast<Error*>(evaluated);
            EXPECT_EQ(errorObj->message, test.expectedMessage) << "wrong error message returned. expected=\""<<test.expectedMessage<<"\"";
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "no error object deteced. Dynamic cast failed";
        }
    }
}

TEST(EvaluatorTests, TestLetStatements){
    struct {
        std::string input;
        int expectedValue;
    } tests[] = {
        {"let a = 5; 5;", 5},
        {"let a = 5 * 5; a;", 25},
        {"let a = 5; let b = a; b;", 5},
        {"let a = 5; let b = a; let c = a + b + 5; c;", 15},
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        testIntegerObject(evaluated, test.expectedValue);
    }
}

TEST(EvaluatorTests, FunctionObject){
    std::string input = "fn(x) { x+2; };";

    Object* evaluated = testEval(input);
    try{
        Function* funcObj = dynamic_cast<Function*>(evaluated);
        ASSERT_EQ(funcObj->parameters.size(), 1) << "function has wrong parameters, expected 1. got="<<funcObj->parameters.size();
        EXPECT_EQ(funcObj->parameters[0]->toString(), "x") << "parameter is not 'x'. got=" << funcObj->parameters[0]->toString();
        std::string expectedBody = "(x + 2)";
        EXPECT_EQ(funcObj->body->toString(), expectedBody) << "body is not "<< expectedBody <<". got=" <<funcObj->body->toString();
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Object is not Function object. Dynamic Cast failed";
    }
}

TEST(EvaluatorTests, FunctionApplication){
    struct {
        std::string input;
        int expected;
    } tests[] = {
        {"let identity = fn(x) { x; }; identity(5);", 5},
        {"let identity = fn(x) { return x; }; identity(5);", 5},
        {"let double = fn(x) { x * 2; }; double(5);", 10},
        {"let add = fn(x, y) { x + y; }; add(5, 5);", 10},
        {"let add = fn(x, y) { x + y; }; add(5 + 5, add(5, 5));", 20},
        {"fn(x) { x; }(5)", 5},
    };

    for(auto test:tests){
        testIntegerObject(testEval(test.input), test.expected);
    }
}

TEST(EvaluatorTests, TestClosures) {
    std::string input = "\
    let newAdder = fn(x) {\
    fn(y) { x + y };\
    };\
    \
    let addTwo = newAdder(2);\
    addTwo(2);";

    testIntegerObject(testEval(input), 4);
}

TEST(EvaluatorTests, TestStringLiteral) {
    std::string input = "\"Hello world!\";";
    Object* evaluated = testEval(input);
    try{
        String* str = dynamic_cast<String*>(evaluated);
        ASSERT_EQ(str->value, "Hello world!") << "String has wrong value. got=" << str->value;
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Object is not String*. Dynamic cast failed";
    }
}

TEST(EvaluatorTests, TestStringConcatentation) {
    std::string input = "\"Hello\" + \" world!\"";
    Object* evaluated = testEval(input);
    try{
        String* str = dynamic_cast<String*>(evaluated);
        ASSERT_EQ(str->value, "Hello world!") << "String has wrong value. got=" << str->value;
    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Object is not String*. Dynamic cast failed";
    }
}

TEST(EvaluatorTests, TestBuiltinFunctions){
    struct {
        std::string input;
        int expectedVal;
        std::string expectedError;
    } tests[] = {
        {"len(\"\")", 0, ""},
        {"len(\"four\")", 4, ""},
        {"len(\"hello world\")", 11, ""},
        {"len(1)", 0, "argument to 'len' not supported, got INTEGER"},
        {"len(\"one\", \"two\")", 0, "wrong number of arguments. expected=1, got=2"},
        {"len([1, 2, 3])", 3, ""},
        {"len([])", 0, ""},
        {"first([1, 2, 3, 4])", 1, ""},
        {"first(1)", 0, "argument to 'first' must be ARRAY, got INTEGER"},
        {"first(\"one\", \"two\")", 0, "wrong number of arguments. expected=1, got=2"},
        {"last([1, 2, 3, 4])", 4, ""},
        {"last(1)", 0, "argument to 'last' must be ARRAY, got INTEGER"},
        {"last(\"one\", \"two\")", 0, "wrong number of arguments. expected=1, got=2"},
        {"rest([1, 2, 3, 4])[0];", 2, ""},
        {"rest([1, 2, 3])[1];", 3, ""},
        {"rest(rest([1,2, 3]))[0]", 3, ""},
        {"let x = [1, 2, 3];\
        let y = rest(x);\
        x[0];", 1, ""
        },
        {"push([1, 2, 3], 4)[3];", 4, ""},
        {"let x = [1, 2, 3]; let y = push(x, 5); y[3]", 5, ""},
        {"let x = [1, 2, 3]; let y = push(x, 5);\
        let x = push(x, 7) x[3]", 7, ""},
    };

    for(auto test:tests){
        Object* evaluated = testEval(test.input);
        if(test.expectedError != ""){
            try{
                Error* err = dynamic_cast<Error*>(evaluated);
                ASSERT_EQ(err->message, test.expectedError) << "Wrong error message";
            }
            catch(const std::bad_cast& e){
                ADD_FAILURE() <<" expected error type did not get. Dynamic cast failed.";
            }
        }
        else
            testIntegerObject(evaluated, test.expectedVal);
    }
}

TEST(EvaluatorTests, TestArrayLiterals){
  std::string input = "[1, 2*2, 3+3]";
    Object* evaluated = testEval(input);
    try{
        Array* ar = dynamic_cast<Array*>(evaluated);
        ASSERT_EQ(ar->elements.size(), 3) << "Array has wrong number of elements. expected 3, got=" << ar->elements.size();
        testIntegerObject(ar->elements[0], 1);
        testIntegerObject(ar->elements[1], 4);
        testIntegerObject(ar->elements[2], 6);

    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Object is not Array*. Dynamic cast failed";
    }
}

TEST(EvaluatorTests, TestArrayIndexing){
    struct {
        std::string input;
        int expected;
    } tests[] = {
        {
            "[1, 2, 3][0]",
            1,
        },
        {
            "[1, 2, 3][1]",
            2,
        },
        {
            "[1, 2, 3][2]",
            3,
        },
        {
            "let i = 0; [1][i];",
            1,
        },
        {
            "[1, 2, 3][1 + 1];",
            3,
        },
        {
            "let myArray = [1, 2, 3]; myArray[2];",
            3,
        },
        {
            "let myArray = [1, 2, 3]; myArray[0] + myArray[1] + myArray[2];",
            6,
        },
        {
            "let myArray = [1, 2, 3]; let i = myArray[0]; myArray[i]",
            2,
        },
        {
            "[1, 2, 3][3]",
            -1, // used to signifiy nullptrs
        },
        {
            "[1, 2, 3][-1]",
            -1, // used to signify nullpts
        },
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        if(test.expected != -1){
            testIntegerObject(evaluated, test.expected);
        }
        else{
            testNullObject(evaluated);
        }
    }
}

TEST(EvaluatorTests, TestHashLiterals){
  std::string input = "let two = \"two\";\
    {\
       \"one\": 10 - 9,\
        two: 1 + 1,\
       \"thr\" + \"ee\": 6 / 2,\
        4: 4,\
        true: 5,\
        false: 6\
    }";
    Object* evaluated = testEval(input);
    try{
        Hash* hash = dynamic_cast<Hash*>(evaluated);
        std::unordered_map<HashKey, int> expected = {
            {String("one").hashKey(), 1},
            {String("two").hashKey(), 2},
            {String("three").hashKey(), 3},
            {Integer(4).hashKey(), 4},
            {TRUE.hashKey(), 5},
            {FALSE.hashKey(), 6},
        };
        ASSERT_EQ(hash->pairs.size(), expected.size()) << "hash has wrong number of pairs. got=" << hash->pairs.size();
        for(auto expectIt: expected){
            std::unordered_map<HashKey, HashPair>::iterator evalIt = hash->pairs.find(expectIt.first);
            ASSERT_NE(evalIt, hash->pairs.end()) << "no pair for given key, expected val=" << expectIt.second;
            testIntegerObject(evalIt->second.value, expectIt.second);
        }

    }
    catch(const std::bad_cast& e){
        ADD_FAILURE() << "Object is not Hash*. Dynamic cast failed";
    }
}

TEST(EvaluatorTests, TestHashIndexing){
    struct {
        std::string input;
        int expected;
    } tests[] = {
        {
            "{\"foo\": 5}[\"foo\"]",
            5,
        },
        {
            "{\"foo\": 5}[\"bar\"]",
            -1,
        },
        {
            "let key = \"foo\"; {\"foo\": 5}[key]",
            5,
        },
        {
            "{}[\"foo\"]",
            -1,
        },
        {
            "{5: 5}[5]",
            5,
        },
        {
            "{true: 5}[true]",
            5,
        },
        {
            "{false: 5}[false]",
            5,
        },
    };
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        if(test.expected != -1){
            testIntegerObject(evaluated, test.expected);
        }
        else{
            testNullObject(evaluated);
        }
    }
}


// Object Tests
TEST(ObjectTests, TestStringHashKey){
    String* hello1 =  new String("Hello World");
    String* hello2 =  new String("Hello World");
    String* name1 =  new String("name");
    String* name2 =  new String("name");

    EXPECT_EQ(hello1->hashKey(), hello2->hashKey()) << "strings with same content have different hash keys";
    EXPECT_EQ(name1->hashKey(), name2->hashKey())<< "strings with same content have different hash keys";
    EXPECT_NE(hello1->hashKey(), name1->hashKey()) << "strings with different content have same hash keys";

    delete hello1;
    delete hello2;
    delete name1;
    delete name2;
}

TEST(ObjectTests, TestBooleanHashKey){
    BooleanObj* true1 =  new BooleanObj(true);
    BooleanObj* true2 =  new BooleanObj(true);
    BooleanObj* false1 = new BooleanObj(false);
    BooleanObj* false2 = new BooleanObj(false);

    EXPECT_EQ(true1->hashKey(), true2->hashKey()) << "bools with same content have different hash keys";
    EXPECT_EQ(false1->hashKey(), false2->hashKey())<< "bools with same content have different hash keys";
    EXPECT_NE(true1->hashKey(), false2->hashKey()) << "bools with different content have same hash keys";

    delete true1;
    delete true2;
    delete false1;
    delete false2;
}

TEST(ObjectTests, TestMixedHashKey){
    Integer* true1 =  new Integer(1);
    Integer* true2 =  new Integer(3);
    BooleanObj* bool1 =  new BooleanObj(true);
    String* hello1 =  new String("3");

    EXPECT_NE(true1->hashKey(), true2->hashKey()) << "ints with same content have different hash keys";
    EXPECT_NE(true1->hashKey(), bool1->hashKey())<< "different types hashing to same value";
    EXPECT_NE(true2->hashKey(), hello1->hashKey()) << "different types hashing to same value";

    delete true1;
    delete true2;
    delete bool1;
    delete hello1;
}

TEST(ObjectTests, TestIntegerHashKey){
    Integer* true1 =  new Integer(1);
    Integer* true2 =  new Integer(1);
    Integer* false1 = new Integer(3);
    Integer* false2 = new Integer(3);

    EXPECT_EQ(true1->hashKey(), true2->hashKey()) << "ints with same content have different hash keys";
    EXPECT_EQ(false1->hashKey(), false2->hashKey())<< "ints with same content have different hash keys";
    EXPECT_NE(true1->hashKey(), false2->hashKey()) << "ints with different content have same hash keys";

    delete true1;
    delete true2;
    delete false1;
    delete false2;
}