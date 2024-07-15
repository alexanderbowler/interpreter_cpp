#include "environment.h"

// gets the value from map returns nullptr if it doesn't exist
Object* Environment::get(std::string& name){
    auto index = store.find(name);
    if(index == store.end()){
        if(outer == nullptr)
            return nullptr;
        return outer->get(name);
    }
    else
        return index->second;
}

// sets the value in the map and returns the value as well
Object* Environment::set(std::string& name, Object* value){
    store[name] = value;
    return value;
}