#include <gtest/gtest.h>
#include "lexer.h"
#include "parser.h"
#include "object.h"
#include <string>
#include <variant>
#include "evaluator.h"

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

// parse helper functions
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
    };
    for( int i = 0; i < 22; i++){
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

// Evaluator helper functions:
Object* testEval(std::string& input){
    Lexer l = Lexer(input);
    Parser p = Parser(&l);
    Program* program = p.parseProgram();

    return Eval(program);
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
        cout<<"here";
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
    };
        
    for(auto test: tests){
        Object* evaluated = testEval(test.input);
        try{
            Error* errorObj = dynamic_cast<Error*>(evaluated);
            EXPECT_EQ(errorObj->message, test.expectedMessage) << "wrong error message returned. expected="<<test.expectedMessage;
        }
        catch(const std::bad_cast& e){
            ADD_FAILURE() << "no error object deteced. Dynamic cast failed";
        }
    }
}