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
