#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "object.h"
#include "parser.h"

// global const vars for True and False
static BooleanObj TRUE = BooleanObj(true);
static BooleanObj FALSE = BooleanObj(false);
static Null NULLOBJ = Null();

// main evaulator function to evaluate the nodes within the AST
Object* Eval(Node* node);

// helper function to evaluate a bunch of statements within a vector of statements
Object* evalStatements(std::vector<Statement*>& stmts);

//helper function which returns the const vars above
BooleanObj* nativeBoolToBooleanObject(bool value);

// helper function to evaluate prefix expressions
Object* evalPrefixExpression(std::string op, Object* operand);

// helper function which applies the ! to the operand
Object* evalBangOperator(Object* operand);

// helper function which applies the - operand to negate a number 
Object* evalMinusPrefixOperator(Object* operand);

#endif