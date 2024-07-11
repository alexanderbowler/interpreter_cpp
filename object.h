// object for evaluator of monkey type, wrapper on all data

#ifndef OBJECT_H
#define OBJECT_H

#include <string>

enum class ObjectType : uint8_t {
    INTEGER_OBJ,
    BOOLEAN_OBJ,
    NULL_OBJ,
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

#endif