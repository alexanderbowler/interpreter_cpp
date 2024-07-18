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

// returns the value of the function as a string
std::string Array::inspect() {
    std::string output = "[";
    for(size_t i = 0; i < elements.size(); i++){
        output += elements[i]->inspect();
        if(i+1 < elements.size())
            output += ", ";
    }
    output += "]";
    return output;
}

// returns the object type of this particular object BUILTIN_OBJ
ObjectType Array::type() {
    return ObjectType::ARRAY_OBJ;
}

// returns the hash of the object
HashKey BooleanObj::hashKey(){
    int hashVal;
    if(value == true)
        hashVal = 1;
    else
        hashVal = 0;
    return HashKey{type(), hashVal};
}

// returns the hash of the object
HashKey String::hashKey(){
    std::hash<std::string> hasher;
    return HashKey{type(), (int)hasher(value)};
}

bool operator==(const HashKey& lhs, const HashKey& rhs){
    if(lhs.type == rhs.type && lhs.value == rhs.value)
        return true;
    else
        return false;
}

bool operator!=(const HashKey& lhs, const HashKey& rhs){
    return !(lhs == rhs);
}

// returns the hash of the object
HashKey Integer::hashKey(){
    return HashKey{type(), value};
}

// returns the value of the function as a string
std::string Hash::inspect() {
    std::string output = "{";
    for(auto it = pairs.begin(); it != pairs.end(); it++){
        output += it->second.key->inspect() + ": " + it->second.value->inspect();
        auto next = it;
        if(++next != pairs.end())
            output += ", ";
    }
    output += "}";
    return output;
}

// returns the object type of this particular object BUILTIN_OBJ
ObjectType Hash::type() {
    return ObjectType::HASH_OBJ;
}

std::size_t std::hash<HashKey>::operator()(const HashKey& k) const{
    return ((std::hash<std::string>()(ObjectTypeToString[k.type]) >> 3) ^ (size_t)k.value);
}

// returns if the object pointer is hashable
bool hashable(Object* obj){
    return (obj->type() == ObjectType::STRING_OBJ ||
            obj->type() == ObjectType::INTEGER_OBJ ||
            obj->type() == ObjectType::BOOLEAN_OBJ);
}