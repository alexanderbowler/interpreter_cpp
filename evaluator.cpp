#include "lexer.h"
#include "parser.h"
#include "object.h"
#include "evaluator.h"
#include <typeinfo>
#include <iostream>

Object* Eval(Node* node){
    const std::type_info& node_type = typeid(*node);
    // if-else switch statement
    
    //statements
    if(node_type == typeid(Program)){
        Program* program = dynamic_cast<Program*>(node);
        return evalStatements(program->statements);
    }
    else if(node_type == typeid(ExpressionStatement)){
        ExpressionStatement* expStmt = dynamic_cast<ExpressionStatement*>(node);
        return Eval(expStmt->expressionValue);
    }
    //expressions
    else if(node_type == typeid(IntegerLiteral)){
        IntegerLiteral* intLit = dynamic_cast<IntegerLiteral*>(node);
        Integer* int_obj = new Integer(intLit->value);
        return int_obj;
    }
    else if(node_type == typeid(Boolean)){
        Boolean* boolLiteral = dynamic_cast<Boolean*>(node);
        return nativeBoolToBooleanObject(boolLiteral->value);
    }
    else if(node_type == typeid(PrefixExpression)){
        PrefixExpression* prefixExp = dynamic_cast<PrefixExpression*>(node);
        Object* right = Eval(prefixExp->right);
        return evalPrefixExpression(prefixExp->op, right);
    }
    else if(node_type == typeid(InfixExpression)){
        InfixExpression* infixExp = dynamic_cast<InfixExpression*>(node);
        Object* left = Eval(infixExp->left);
        Object* right = Eval(infixExp->right);
        return evalInfixExpression(infixExp->op, left, right);
    }

    return nullptr;

}

Object* evalStatements(std::vector<Statement*>& stmts){
    Object* result = nullptr;
    for(Statement* stmt: stmts){
        result = Eval(stmt);
    }
    return result;
}

//helper function which returns the const vars for bools
BooleanObj* nativeBoolToBooleanObject(bool value){
    if(value)
        return &TRUE;
    return &FALSE;
}

// helper function to evaluate prefix expressions
Object* evalPrefixExpression(std::string op, Object* operand){
    if(op == "!"){
        return evalBangOperator(operand);
    }
    else if(op == "-"){
        return evalMinusPrefixOperator(operand);
    }
    else
        return &NULLOBJ;
}

// helper function which applies the ! to the operand
Object* evalBangOperator(Object* operand){
    const std::type_info& operand_type = typeid(*operand);
    if(operand_type == typeid(BooleanObj)){
        BooleanObj* op = dynamic_cast<BooleanObj*>(operand);
        if(op == &TRUE){
            return &FALSE;
        }
        else{
            return &TRUE;
        }
    }
    else if(operand_type == typeid(NULLOBJ)){
        return &TRUE;
    }
    else
        return &FALSE;
    
}

// helper function which applies the - operand to negate a number 
Object* evalMinusPrefixOperator(Object* operand){
    if(typeid(*operand) == typeid(Integer)){
        Integer* right_int = dynamic_cast<Integer*>(operand);
        int val = right_int->value;
        return new Integer(-val);
    }
    else{
        return &NULLOBJ;
    }
}

// helper function to evaluate infix statements and return their value
Object* evalInfixExpression(std::string op, Object* left, Object* right){
    const std::type_info& left_type = typeid(*left);
    const std::type_info& right_type = typeid(*right);
    if(left_type == typeid(Integer) && right_type == typeid(Integer)){
        Integer* left_int = dynamic_cast<Integer*>(left);
        Integer* right_int = dynamic_cast<Integer*>(right);
        return evalIntegerInfixExpression(op, left_int, right_int);
    }
    else if(op == "=="){
        return nativeBoolToBooleanObject(left == right);
    }
    else if(op == "!="){
        return nativeBoolToBooleanObject(left != right);
    }
    else
        return &NULLOBJ;

}

// helper function to evaluate infix statements of two integers
Object* evalIntegerInfixExpression(std::string op, Integer* left_int, Integer* right_int){
    int left_val = left_int->value;
    int right_val = right_int->value;
    if(op == "+")
        return new Integer(left_val+right_val);
    else if(op == "-")
        return new Integer(left_val-right_val);
    else if(op == "*")
        return new Integer(left_val*right_val);
    else if(op == "/")
        return new Integer(left_val/right_val);
    else if(op == "<")
        return nativeBoolToBooleanObject(left_val<right_val);
    else if(op == ">")
        return nativeBoolToBooleanObject(left_val>right_val);
    else if(op == "==")
        return nativeBoolToBooleanObject(left_val==right_val);
    else if(op == "!=")
        return nativeBoolToBooleanObject(left_val!=right_val);
    else
        return &NULLOBJ;
}