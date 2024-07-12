#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "object.h"
#include "parser.h"

// global const vars for True and False
static BooleanObj TRUE = BooleanObj(true);
static BooleanObj FALSE = BooleanObj(false);
static Null NULLOBJ = Null();

static std::unordered_map<ObjectType, std::string> ObjectTypeToString = {
    {ObjectType::INTEGER_OBJ, "INTEGER"},
    {ObjectType::BOOLEAN_OBJ, "BOOLEAN"},
    {ObjectType::NULL_OBJ, "NULL"},
    {ObjectType::RETURN_VALUE_OBJ, "RETURN_VALUE"},
    {ObjectType::ERROR_OBJ, "ERROR"},
};

// main evaulator function to evaluate the nodes within the AST
Object* Eval(Node* node);

// helper function to evaluate a all the statements within a program
Object* evalProgram(std::vector<Statement*>& stmts);

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
Object* evalIfExpression(IfExpression* exp);

// helper function to tell if a condition is truthy or not
bool isTruthy(Object* obj);

// helper function to evaluate a block of statements taking care of returns
Object* evalBlockStatement(std::vector<Statement*>& stmts);

// helper function for creating error objects
Error* newError(std::string message);

// helper function to check if an object is an error
bool isError(Object* obj);

#endif