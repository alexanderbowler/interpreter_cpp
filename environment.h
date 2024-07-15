
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include "object.h"

class Environment {
    public:
        //constructor for base level enviroment, is the outer most environment
        Environment(){
            outer = nullptr;
            store = std::unordered_map<std::string, Object*>();
        }
        
        // constructor for inner enclosed enviroment which specifies an environment pointer
        // to the outer environment
        Environment(Environment* outer){
            this->outer = outer;
            store = std::unordered_map<std::string, Object*>();
        }

        // gets the value from map returns nullptr if it doesn't exist
        Object* get(std::string& name);

        // sets the value in the map and returns the value as well
        Object* set(std::string& name, Object* value);


    //vars
    private:
        Environment* outer;
        std::unordered_map<std::string, Object*> store;
};


#endif