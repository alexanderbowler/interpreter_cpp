// main runner for interpreter

#include <iostream>
#include <string>
#include "repl.h"

int main(){
    REPL repl;
    std::cout<<"Welcome to the Monkey programming language REPL!"<<std::endl;
    repl.start();
    return 0;
}