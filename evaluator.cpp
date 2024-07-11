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