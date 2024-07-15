// repl.h definitions
#include "repl.h"

void printParserErrors(Parser& p);

// REPL constructor
// EFFECTS:  creates a REPL object
REPL::REPL(){
    lexer = Lexer();
};


// Start the REPL
// EFFECTS:  starts the REPL
void REPL::start(){
    Environment env = Environment();

    while(true){
        std::cout << PROMPT;
        std::string input;
        std::getline(std::cin, input);
        if(input == "")
            return;
        lexer = Lexer(input);
        parser = Parser(&lexer);
        Program* program = parser.parseProgram();

        if(parser.errors.size() != 0){
            printParserErrors(parser);
            continue;
        }

        Object* evaluated = Eval(program, &env);
        if(evaluated){
            std::cout<<evaluated->inspect()<<"\n";
        }
    }
}

void printParserErrors(Parser& p){
    std::cout<<"ERRORS:\n\tParser Errors:\n";
    for(std::string error:p.errors){
        std::cout<<"\t"<<error<<"\n";
    }
}