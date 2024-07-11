// REPL for monkeytype interpreter

#ifndef REPL_H
#define REPL_H

#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"



class REPL {
    public:
        const std::string PROMPT = ">> ";
        // REPL constructor
        // EFFECTS:  creates a REPL object
        REPL();

        // Start the REPL
        // EFFECTS:  starts the REPL
        void start();

    private:

        std::string input;
        Lexer lexer;
        Parser parser;
};


const std::string PROMPT = ">> ";


#endif // REPL_H