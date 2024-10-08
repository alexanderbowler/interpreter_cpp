#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "object.h"
#include "parser.h"
#include "environment.h"

// global const vars for True and False
static BooleanObj TRUE = BooleanObj(true);
static BooleanObj FALSE = BooleanObj(false);
static Null NULLOBJ = Null();



// len function wrapper using Object* 
// REQUIRES:    input[0] be the actual input to the length function
Object* objectLength(std::vector<Object*> input);

// global map for builtin functions
extern std::unordered_map<std::string, Builtin*> builtins;

// main evaulator function to evaluate the nodes within the AST
Object* Eval(Node* node, Environment* env);

// helper function to evaluate a all the statements within a program
Object* evalProgram(std::vector<Statement*>& stmts, Environment* env);

//helper function which returns the const vars above
BooleanObj* nativeBoolToBooleanObject(bool value);

// helper function to evaluate prefix expressions
Object* evalPrefixExpression(std::string op, Object* operand);

// helper function which applies the ! to the operand
Object* evalBangOperator(Object* operand);

// helper function which applies the - operand to negate a number 
Object* evalMinusPrefixOperator(Object* operand);

// helper function to evaluate infix statements and return their value
Object* evalInfixExpression(std::string op, Object* left, Object* right);

// helper function to evaluate infix statements of two integers
Object* evalIntegerInfixExpression(std::string op, Integer* left_int, Integer* right_int);

// helper function to evaluate if expressions 
Object* evalIfExpression(IfExpression* exp, Environment* env);

// helper function to tell if a condition is truthy or not
bool isTruthy(Object* obj);

// helper function to evaluate a block of statements taking care of returns
Object* evalBlockStatement(std::vector<Statement*>& stmts, Environment* env);

// helper function for creating error objects
Error* newError(std::string message);

// helper function to check if an object is an error
bool isError(Object* obj);

// helper function which returns the value of an identifier through the enviroment
Object* evalIdentifier(Identifier* ident, Environment* env);

// helper function to evaluate the value of parameters before passing them to functions
std::vector<Object*> evalExpressions(std::vector<Expression*>& params, Environment* env);

// helper function which evaluates the function body of a func given its parameters
Object* applyFunction(Object* function, std::vector<Object*>& args);

// takes in a function* and uses the enviroment to create a new extended enviroment with proper
// params passed through and returns that
Environment* extendFunctionEnv(Function* func, std::vector<Object*> args);

// helper function which unwraps the return value for function evaluation
Object* unwrapReturnValue(Object* evaluated);

// helper function for doing string concatentation
Object* evalStringInfixExpression(std::string op, Object* left, Object* right);

// helper function which checks if its properly an array and integer
Object* evalIndexExpression(Object* left, Object* index);

// helper which accesses the value of the array 
Object* evalArrayIndexExpression(Array* left, Integer* index);

// builtin function FIRST for arrays gets the first element of the array
Object* first(std::vector<Object*> inputs);

// builtin function LAST for arrays gets the last element of the array
Object* last(std::vector<Object*> inputs);

// builtin function REST for arrays gets a copy of the array with all but first element
Object* rest(std::vector<Object*> inputs);

// builtin function PUSH for arrays gets a copy of the array and adds desired element to the end
Object* push(std::vector<Object*> inputs);

// evaluation function which evaluates a hash literal
Object* evalHashLiteral(HashLiteral* hashLit, Environment* env);

// function to evaluate indexing into a hash object
 Object* EvalHashIndexExpression(Hash* hash, Object* index);

 // builtin function puts for printing to the screen
Object* puts(std::vector<Object*> inputs);

#endif