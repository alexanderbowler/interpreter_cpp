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
        return evalProgram(program->statements);
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
        if(isError(right))
            return right;
        return evalPrefixExpression(prefixExp->op, right);
    }
    else if(node_type == typeid(InfixExpression)){
        InfixExpression* infixExp = dynamic_cast<InfixExpression*>(node);
        Object* left = Eval(infixExp->left);
        if(isError(left))
            return left;

        Object* right = Eval(infixExp->right);
        if(isError(right))
            return right;

        return evalInfixExpression(infixExp->op, left, right);
    }
    else if(node_type == typeid(BlockStatement)){
        BlockStatement* block = dynamic_cast<BlockStatement*>(node);
        return evalBlockStatement(block->statements);
    }
    else if(node_type == typeid(IfExpression)){
        IfExpression* ifExp = dynamic_cast<IfExpression*>(node);
        return evalIfExpression(ifExp);
    }
    else if(node_type == typeid(ReturnStatement)){
        ReturnStatement* returnStmt = dynamic_cast<ReturnStatement*>(node);
        Object* result = Eval(returnStmt->expressionValue);
        if(isError(result))
            return result;

        return new ReturnValue(result);
    }

    return nullptr;

}

Object* evalProgram(std::vector<Statement*>& stmts){
    Object* result = nullptr;
    for(Statement* stmt: stmts){
        result = Eval(stmt);

        if(result != nullptr && typeid(*result) == typeid(ReturnValue)){
            ReturnValue* returnVal = dynamic_cast<ReturnValue*>(result);
            return returnVal->value;
        }
        if(typeid(*result) == typeid(Error))
            return result;
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
    else{
        return newError("unknown operator: " + op + " " + ObjectTypeToString[operand->type()]);
    }
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
        return newError("unknown operator: -"+ObjectTypeToString[operand->type()]);
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
    else if(left_type != right_type){
        return newError("type mismatch: " + ObjectTypeToString[left->type()] +
        " " + op + " " + ObjectTypeToString[right->type()]);
    }
    else if(op == "=="){
        return nativeBoolToBooleanObject(left == right);
    }
    else if(op == "!="){
        return nativeBoolToBooleanObject(left != right);
    }
    else
        return newError("unknown operator: " + ObjectTypeToString[left->type()] +
        " " + op + " " + ObjectTypeToString[right->type()]);

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
        return newError("unknown operator: " + ObjectTypeToString[left_int->type()] + " "
        + op + " " + ObjectTypeToString[right_int->type()]);
}

// helper function to evaluate if expressions 
Object* evalIfExpression(IfExpression* exp){
    Object* condition = Eval(exp->condition);
    if(isError(condition))
        return condition;
    if(isTruthy(condition)){
        return Eval(exp->consequence);
    }
    else if(exp->alternative != nullptr)
        return Eval(exp->alternative);
    else {
        return &NULLOBJ;

    }
}

// helper function to tell if a condition is truthy or not
bool isTruthy(Object* obj){
    if(obj == &NULLOBJ)
        return false;
    else if(obj == &TRUE)
        return true;
    else if(obj == &FALSE)
        return false;
    else
        return true;
}

// helper function to evaluate a block of statements taking care of returns
Object* evalBlockStatement(std::vector<Statement*>& stmts){
    Object* result = nullptr;
    for(Statement* stmt: stmts){
        result = Eval(stmt);

        if(result != nullptr && (typeid(*result) == typeid(ReturnValue) || typeid(*result) == typeid(Error))){
            return result;
        }
    }

    return result;
}

// helper function for creating error objects
Error* newError(std::string message){
    return new Error(message);
}

// helper function to check if an object is an error
bool isError(Object* obj){
    if(obj != nullptr)
        return obj->type() == ObjectType::ERROR_OBJ;
        
    return false;
}