#include <gtest/gtest.h>
#include "lexer.h"
#include "parser.h"
#include <string>
#include <variant>

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



// Tests for the Lexer
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

// Tests for the Parser
TEST(ParserTests, LetStatementsTest) {
    string input = "let x =  5;\
    let y = 10;\
    let foobar = 838383;";

    Lexer lexer = Lexer(input);
    Parser parser = Parser(&lexer);

    Program* program = parser.parseProgram();
    checkParserErrors(parser);
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

TEST(ParserTests, ReturnStatementsTest) {
    string input = "return 5;\
    return 10;\
    return add(10);";

    Lexer lexer = Lexer(input);
    Parser parser = Parser(&lexer);

    Program* program = parser.parseProgram();
    checkParserErrors(parser);
    ASSERT_NE(program, nullptr) << "ERROR MSG: ParseProgram() returned nullptr";

    ASSERT_EQ(program->statements.size(), 3) 
    << "ERROR MSG: Program.statements does not contain 3 statements got=" << program->statements.size();

    for(int i = 0; i < 3; i++){
        Statement* stmt = program->statements[i];
        if(testReturnStatement(stmt) == false){
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

// Parser Test Helper Function implementations
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