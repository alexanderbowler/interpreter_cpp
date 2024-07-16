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
};

class Object {
    public:
        virtual ObjectType type() = 0;

        virtual std::string inspect() = 0;
};

class Integer: public Object {
    public:
    //constructor
    Integer(int val): value(val){}

    // returns the value of the intger as a string
    std::string inspect() override;

    // returns the object type of this particular object INTEGER
    ObjectType type() override;

    // vars
        int value;
};

class BooleanObj: public Object {
    public:
    // constructor
    BooleanObj(bool val): value(val){}

    // returns the value of the intger as a string
    std::string inspect() override;

    // returns the object type of this particular object INTEGER
    ObjectType type() override;

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

#endif