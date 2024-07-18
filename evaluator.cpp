#include "lexer.h"
#include "parser.h"
#include "object.h"
#include "evaluator.h"
#include <typeinfo>
#include <iostream>

std::unordered_map<std::string, Builtin*>builtins = {
    {"len",  new Builtin(&objectLength)},
    {"first", new Builtin(&first)},
    {"last", new Builtin(&last)},
    {"rest", new Builtin(&rest)},
    {"push", new Builtin(&push)}
};

Object* Eval(Node* node, Environment* env){
    const std::type_info& node_type = typeid(*node);
    // if-else switch statement
    
    //statements
    if(node_type == typeid(Program)){
        Program* program = dynamic_cast<Program*>(node);
        return evalProgram(program->statements, env);
    }
    else if(node_type == typeid(ExpressionStatement)){
        ExpressionStatement* expStmt = dynamic_cast<ExpressionStatement*>(node);
        return Eval(expStmt->expressionValue, env);
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
        Object* right = Eval(prefixExp->right, env);
        if(isError(right))
            return right;
        return evalPrefixExpression(prefixExp->op, right);
    }
    else if(node_type == typeid(InfixExpression)){
        InfixExpression* infixExp = dynamic_cast<InfixExpression*>(node);
        Object* left = Eval(infixExp->left, env);
        if(isError(left))
            return left;

        Object* right = Eval(infixExp->right, env);
        if(isError(right))
            return right;

        return evalInfixExpression(infixExp->op, left, right);
    }
    else if(node_type == typeid(BlockStatement)){
        BlockStatement* block = dynamic_cast<BlockStatement*>(node);
        return evalBlockStatement(block->statements, env);
    }
    else if(node_type == typeid(IfExpression)){
        IfExpression* ifExp = dynamic_cast<IfExpression*>(node);
        return evalIfExpression(ifExp, env);
    }
    else if(node_type == typeid(ReturnStatement)){
        ReturnStatement* returnStmt = dynamic_cast<ReturnStatement*>(node);
        Object* result = Eval(returnStmt->expressionValue, env);
        if(isError(result))
            return result;

        return new ReturnValue(result);
    }
    else if(node_type == typeid(LetStatement)){
        LetStatement* letStmt = dynamic_cast<LetStatement*>(node);
        Object* val = Eval(letStmt->expressionValue, env);
        if(isError(val))
            return val;
        env->set(letStmt->name->value, val);

    }
    else if(node_type == typeid(Identifier)){
        Identifier* ident = dynamic_cast<Identifier*>(node);
        return evalIdentifier(ident, env);
    }
    else if(node_type == typeid(FunctionLiteral)){
        FunctionLiteral* funcLit = dynamic_cast<FunctionLiteral*>(node);
        return new Function(funcLit->parameters, funcLit->body, env);
    }
    else if(node_type == typeid(CallExpression)){
        CallExpression* callExp = dynamic_cast<CallExpression*>(node);
        Object* function = Eval(callExp->function, env);
        if(isError(function))
            return function;
        
        std::vector<Object*> args = evalExpressions(callExp->arguments, env);
        if(args.size() == 1 && isError(args[0]))
            return args[0];
        
        return applyFunction(function, args);
    }
    else if(node_type == typeid(StringLiteral)){
        StringLiteral* str = dynamic_cast<StringLiteral*>(node);
        return new String(str->value);
    }
    else if(node_type == typeid(ArrayLiteral)){
        ArrayLiteral* ar = dynamic_cast<ArrayLiteral*>(node);
        std::vector<Object*> elems = evalExpressions(ar->elements, env);
        if(elems.size() == 1 && isError(elems[0])){
            return elems[0];
        }
        return new Array(elems);
    }
    else if(node_type == typeid(IndexExpression)){
        IndexExpression* indexExp = dynamic_cast<IndexExpression*>(node);
        Object* left = Eval(indexExp->left, env);
        if(isError(left))
            return left;
        Object* index = Eval(indexExp->index, env);
        if(isError(index))
            return index;
        return evalIndexExpression(left, index);
    }
    else if(node_type == typeid(HashLiteral)){
        HashLiteral* hashLit = dynamic_cast<HashLiteral*>(node);
        return evalHashLiteral(hashLit, env);
    }
    return nullptr;

}

Object* evalProgram(std::vector<Statement*>& stmts, Environment* env){
    Object* result = nullptr;
    for(Statement* stmt: stmts){
        result = Eval(stmt, env);

        if(result != nullptr && typeid(*result) == typeid(ReturnValue)){
            ReturnValue* returnVal = dynamic_cast<ReturnValue*>(result);
            return returnVal->value;
        }
        if(result != nullptr && typeid(*result) == typeid(Error))
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
    else if(left_type == typeid(String) && right_type == typeid(String)){
        return evalStringInfixExpression(op, left, right);
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
Object* evalIfExpression(IfExpression* exp, Environment* env){
    Object* condition = Eval(exp->condition, env);
    if(isError(condition))
        return condition;
    if(isTruthy(condition)){
        return Eval(exp->consequence, env);
    }
    else if(exp->alternative != nullptr)
        return Eval(exp->alternative, env);
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
Object* evalBlockStatement(std::vector<Statement*>& stmts, Environment* env){
    Object* result = nullptr;
    for(Statement* stmt: stmts){
        result = Eval(stmt, env);

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

// helper function which returns the value of an identifier through the enviroment
Object* evalIdentifier(Identifier* ident, Environment* env){
    Object* val = env->get(ident->value);
    if(val == nullptr){ // not a variable in our environment
        // first check if its a builtin func name
        auto funcIt = builtins.find(ident->value);
        if(funcIt == builtins.end()) //not a builtin function
            return newError("identifier not found: " + ident->value);
        else // is a builtin function
            return funcIt->second; // return the Builtin object which has a function pointer to proper func
    }
    return val;
}

// helper function to evaluate the value of parameters before passing them to functions
std::vector<Object*> evalExpressions(std::vector<Expression*>& params, Environment* env){
    std::vector<Object*> result;
    for(Expression* param: params){
        Object* evaluated = Eval(param, env);
        if(isError(evaluated)){
            std::vector<Object*> err = {evaluated};
            return err;
        }
        result.push_back(evaluated);
    }
    return result;
}

// helper function which evaluates the function body of a func given its parameters
Object* applyFunction(Object* uncast_function, std::vector<Object*>& args){
    if(typeid(*uncast_function) == typeid(Function)){
        Function* func = dynamic_cast<Function*>(uncast_function);
        Environment* extendedEnv = extendFunctionEnv(func, args);
        Object* evaluated = Eval(func->body, extendedEnv);
        return unwrapReturnValue(evaluated);
    }
    else if(typeid(*uncast_function) == typeid(Builtin)){
        Builtin* func = dynamic_cast<Builtin*>(uncast_function);
        return func->fn(args);
    }
    
    else{
        return newError("Apply function on not a function");
    }
}

// takes in a function* and uses the enviroment to create a new extended enviroment with proper
// params passed through and returns that
Environment* extendFunctionEnv(Function* func, std::vector<Object*> args){
    Environment* extendedEnv = new Environment(func->env);
    for(size_t i = 0; i < func->parameters.size(); i++){
        extendedEnv->set(func->parameters[i]->value, args[i]);
    }
    return extendedEnv;
}

// helper function which unwraps the return value for function evaluation
Object* unwrapReturnValue(Object* evaluated){
    if(typeid(*evaluated) == typeid(ReturnValue)){
        ReturnValue* returnVal = dynamic_cast<ReturnValue*>(evaluated);
        return returnVal->value;
    }
        return evaluated;
}

// helper function for doing string concatentation
Object* evalStringInfixExpression(std::string op, Object* left, Object* right){
    String* left_str = dynamic_cast<String*>(left);
    String* right_str = dynamic_cast<String*>(right);

    if( op == "==")
        return nativeBoolToBooleanObject(left_str->value == right_str->value);

    if(op == "!="){
        return nativeBoolToBooleanObject(left_str->value != right_str->value);
    }
    if( op != "+")
        return newError("unknown operator: " + ObjectTypeToString[left_str->type()]+ " "
        + op + " " + ObjectTypeToString[right_str->type()]);

    return new String(left_str->value + right_str->value);
}


// length function for arrays and strings
Object* objectLength(std::vector<Object*> input){
    if(input.size() != 1)
        return newError("wrong number of arguments. expected=1, got=" + std::to_string(input.size()));
    if(input[0]->type() == ObjectType::STRING_OBJ){
        String* inStr = dynamic_cast<String*>(input[0]);
        size_t length = inStr->value.size();
        return new Integer((int) length); 
    }
    else if(input[0]->type() == ObjectType::ARRAY_OBJ){
        Array* ar = dynamic_cast<Array*>(input[0]);
        size_t length = ar->elements.size();
        return new Integer((int) length); 
    }
        return newError("argument to 'len' not supported, got " + ObjectTypeToString[input[0]->type()]);
    
}

// builtin function FIRST for arrays gets the first element of the array
Object* first(std::vector<Object*> inputs){
    if(inputs.size() != 1)
        return newError("wrong number of arguments. expected=1, got=" + std::to_string(inputs.size()));
    else if(inputs[0]->type() == ObjectType::ARRAY_OBJ){
        Array* ar = dynamic_cast<Array*>(inputs[0]);
        if(ar->elements.size() < 1)
            return &NULLOBJ;
        return ar->elements[0]; 
    }
    else
        return newError("argument to 'first' must be ARRAY, got " + ObjectTypeToString[inputs[0]->type()]);
    
}

// builtin function LAST for arrays gets the last element of the array
Object* last(std::vector<Object*> inputs){
    if(inputs.size() != 1)
        return newError("wrong number of arguments. expected=1, got=" + std::to_string(inputs.size()));
    else if(inputs[0]->type() == ObjectType::ARRAY_OBJ){
        Array* ar = dynamic_cast<Array*>(inputs[0]);
        if(ar->elements.size() < 1)
            return &NULLOBJ;
        return ar->elements[ar->elements.size()-1]; 
    }
    else
        return newError("argument to 'last' must be ARRAY, got " + ObjectTypeToString[inputs[0]->type()]);
    
}

// builtin function REST for arrays gets a copy of the array with all but first element
Object* rest(std::vector<Object*> inputs){
    if(inputs.size() != 1)
        return newError("wrong number of arguments. expected=1, got=" + std::to_string(inputs.size()));
    else if(inputs[0]->type() == ObjectType::ARRAY_OBJ){
        Array* ar = dynamic_cast<Array*>(inputs[0]);
        if(ar->elements.size() < 1)
            return &NULLOBJ;
        std::vector<Object*> tail;
        tail.resize(ar->elements.size()-1);
        std::copy( ++(ar->elements.begin()), ar->elements.end(), tail.begin());
        return new Array(tail); 
    }
    else
        return newError("argument to 'rest' must be ARRAY, got " + ObjectTypeToString[inputs[0]->type()]);
    
}

// builtin function PUSH for arrays gets a copy of the array and adds desired element to the end
Object* push(std::vector<Object*> inputs){
    if(inputs.size() != 2){
        return newError("wrong number of arguments. expected=2, got=" + std::to_string(inputs.size()));
    }
    if(inputs[0]->type() != ObjectType::ARRAY_OBJ){
        return newError("argument to 'push' must be ARRAY, got " + ObjectTypeToString[inputs[0]->type()]);
    }
    Array* ar = dynamic_cast<Array*>(inputs[0]);
    std::vector<Object*> newAr(ar->elements.begin(), ar->elements.end());
    newAr.push_back(inputs[1]);
    return new Array(newAr);
}

// helper function which access the proper element on an array using indexing
Object* evalIndexExpression(Object* left, Object* index){
    if(left->type() == ObjectType::ARRAY_OBJ && index->type() == ObjectType::INTEGER_OBJ){
        Array* ar = dynamic_cast<Array*>(left);
        Integer* idx = dynamic_cast<Integer*>(index);
        return evalArrayIndexExpression(ar, idx);
    }
    else if(left->type() == ObjectType::HASH_OBJ){
        Hash* hash = dynamic_cast<Hash*>(left);
        return EvalHashIndexExpression(hash, index);
    }
    else{
        return newError("index operator not supported: " + ObjectTypeToString[left->type()]);
    }
}

// helper which accesses the value of the array 
Object* evalArrayIndexExpression(Array* ar, Integer* index){
    size_t maxIdx = ar->elements.size() - 1;
    if(index->value < 0 || index->value > (int)maxIdx)
        return &NULLOBJ;
    return ar->elements[(size_t)index->value];
}

// evaluation function which evaluates a hash literal
Object* evalHashLiteral(HashLiteral* hashLit, Environment* env){
    Hash* newHash = new Hash();

    for(auto it : hashLit->pairs){
        Object* key = Eval(it.first, env);
        if(isError(key))
            return key;
        if(!hashable(key))
            return newError("unusable as hash key. type=" + ObjectTypeToString[key->type()]);

        Object* value = Eval(it.second, env);
        if(isError(value))
            return value;

        HashableObject* hashKey = dynamic_cast<HashableObject*>(key);
        HashKey hashed = hashKey->hashKey();
        newHash->pairs[hashed] = HashPair{key, value};
    }
    return newHash;
}

// function to evaluate indexing into a hash object
 Object* EvalHashIndexExpression(Hash* hash, Object* index){
    if(!hashable(index)){
        return newError("unusable as hash key: " + ObjectTypeToString[index->type()]);
    }
    HashableObject* idx = dynamic_cast<HashableObject*>(index);
    auto it = hash->pairs.find(idx->hashKey());
    if(it == hash->pairs.end()){
        return &NULLOBJ;
    }
    return it->second.value;
 }