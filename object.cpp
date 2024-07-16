#include "object.h"
#include <string>

// returns the value of the intger as a string
std::string Integer::inspect(){
    return std::to_string(value);
}

// returns the object type of this particular object INTEGER
ObjectType Integer::type() {
    return ObjectType::INTEGER_OBJ;
}


// returns the value of the intger as a string
std::string BooleanObj::inspect(){
    if(value)
        return "true";
    return "false";
}

// returns the object type of this particular object INTEGER
ObjectType BooleanObj::type() {
    return ObjectType::BOOLEAN_OBJ;
}

 // returns the value of the intger as a string
std::string Null::inspect(){
    return "null";
}

// returns the object type of this particular object INTEGER
ObjectType Null::type() {
    return ObjectType::NULL_OBJ;
}

// returns the value of the intger as a string
std::string ReturnValue::inspect(){
    return value->inspect();
}

// returns the object type of this particular object INTEGER
ObjectType ReturnValue::type() {
    return ObjectType::RETURN_VALUE_OBJ;
}

// returns the value of the intger as a string
std::string Error::inspect() {
    return "ERROR: " + message;
}

// returns the object type of this particular object ERROR
ObjectType Error::type() {
    return ObjectType::ERROR_OBJ;
}

 // returns the value of the intger as a string
std::string Function::inspect()  {
    std::string output = "fn(";
    for(size_t i = 0; i < parameters.size(); i++){
        output += parameters[i]->toString();
        if(i+1 < parameters.size())
            output += ",";
    }
    output += ") {\n";
    output+= body->toString();
    output += "\n}";

    return output;
}

// returns the object type of this particular object FUNCTION_OBJ
ObjectType Function::type() {
    return ObjectType::FUNCTION_OBJ;
}

// returns the value of the string
std::string String::inspect() {
    return value;
}

// returns the object type of this particular object INTEGER
ObjectType String::type() {
    return ObjectType::STRING_OBJ;
}

// returns the value of the function as a string
std::string Builtin::inspect() {
    return "builtin function";
}

// returns the object type of this particular object BUILTIN_OBJ
ObjectType Builtin::type() {
    return ObjectType::BUILTIN_OBJ;
}