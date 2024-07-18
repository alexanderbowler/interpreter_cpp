// object for evaluator of monkey type, wrapper on all data

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "ast.h"

// forward declaration of Environment class
class Environment;


enum class ObjectType : uint8_t {
    INTEGER_OBJ,
    BOOLEAN_OBJ,
    NULL_OBJ,
    RETURN_VALUE_OBJ,
    ERROR_OBJ,
    FUNCTION_OBJ,
    STRING_OBJ,
    BUILTIN_OBJ,
    ARRAY_OBJ,
    HASH_OBJ,
};

static std::unordered_map<ObjectType, std::string> ObjectTypeToString = {
    {ObjectType::INTEGER_OBJ, "INTEGER"},
    {ObjectType::BOOLEAN_OBJ, "BOOLEAN"},
    {ObjectType::NULL_OBJ, "NULL"},
    {ObjectType::RETURN_VALUE_OBJ, "RETURN_VALUE"},
    {ObjectType::ERROR_OBJ, "ERROR"},
    {ObjectType::FUNCTION_OBJ, "FUNCTION"},
    {ObjectType::STRING_OBJ, "STRING"},
    {ObjectType::BUILTIN_OBJ, "BUILTIN"},
    {ObjectType::ARRAY_OBJ, "ARRAY"},
    {ObjectType::HASH_OBJ, "HASH"},
};


// hashkey struct
struct HashKey {
    ObjectType type;
    int value;
};

template <>
struct std::hash<HashKey>{
    std::size_t operator()(const HashKey& k) const;
};



bool operator==(const HashKey& lhs, const HashKey& rhs);

bool operator!=(const HashKey& lhs, const HashKey& rhs);

class Object {
    public:
        virtual ObjectType type() = 0;

        virtual std::string inspect() = 0;
};

// returns if the object pointer is hashable
bool hashable(Object* obj);

// struct for the key value pair the hash goes to 
struct HashPair {
    Object* key;
    Object* value;
};

class HashableObject: public Object{
    public:
        virtual HashKey hashKey() = 0;
};

class Integer: public HashableObject {
    public:
    //constructor
    Integer(int val): value(val){}

    // returns the value of the intger as a string
    std::string inspect() override;

    // returns the object type of this particular object INTEGER
    ObjectType type() override;

    // returns the hash of the object
    HashKey hashKey() override;

    // vars
        int value;
};

class String: public HashableObject {
    public:
    //constructor
    String(std::string val): value(val){}

    // returns the value of the string
    std::string inspect() override;

    // returns the object type of this particular object INTEGER
    ObjectType type() override;

    // returns the hash of the object
    HashKey hashKey() override;

    // vars
        std::string value;
};

class BooleanObj: public HashableObject {
    public:
    // constructor
    BooleanObj(bool val): value(val){}

    // returns the value of the intger as a string
    std::string inspect() override;

    // returns the object type of this particular object INTEGER
    ObjectType type() override;

    // returns the hash of the object
    HashKey hashKey() override;

    // vars
        bool value;
};

class Null: public Object {
    public:

    // returns the value of the intger as a string
    std::string inspect() override;

    // returns the object type of this particular object INTEGER
    ObjectType type() override;
};

class ReturnValue: public Object {
    public:
        //constructor
        ReturnValue(Object* obj): value(obj){}

        // destructor
        ~ReturnValue(){
        }

        // returns the value of the intger as a string
        std::string inspect() override;

        // returns the object type of this particular object from value
        ObjectType type() override;
    
    //vars
    Object* value;
};

class Error: public Object{
    public:
        //constructor
        Error(std::string& msg):  message(msg){}

        // returns the value of the intger as a string
        std::string inspect() override;

        // returns the object type of this particular object ERROR
        ObjectType type() override;
    
    //vars
    std::string message;
};

class Function: public Object{
    public:
    //constructor
    Function(std::vector<Identifier*>& params, BlockStatement* bod, Environment* e): 
    parameters(params), body(bod), env(e){}

    // destructor
    ~Function(){
        delete body;
    }

    // returns the value of the intger as a string
    std::string inspect() override;

    // returns the object type of this particular object FUNCTION_OBJ
    ObjectType type() override;

    //vars
    std::vector<Identifier*> parameters;
    BlockStatement* body;
    Environment* env;
};

class Builtin: public Object{
    public:
    // constructor
    Builtin(Object* (*inFunc)(std::vector<Object*>)){
        fn = inFunc;
    }

    // returns the value of the function as a string
    std::string inspect() override;

    // returns the object type of this particular object BUILTIN_OBJ
    ObjectType type() override;

    //vars 
    Object* (*fn)(std::vector<Object*>); // this is a function pointer outputs an object with input as object is bad!

};

class Array: public Object{
    public:
    // constructor
    Array(std::vector<Object*>& ar){elements = ar;};

    // returns the value of the function as a string
    std::string inspect() override;

    // returns the object type of this particular object BUILTIN_OBJ
    ObjectType type() override;

    //vars
    std::vector<Object*> elements;
};

class Hash: public Object {
    public:
    // constructor
    Hash(){};

    // returns the value of the function as a string
    std::string inspect() override;

    // returns the object type of this particular object BUILTIN_OBJ
    ObjectType type() override;

    //vars
    std::unordered_map<HashKey, HashPair> pairs;
};

#endif