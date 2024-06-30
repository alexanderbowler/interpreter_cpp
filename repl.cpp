// repl.h definitions
#include "repl.h"

// REPL constructor
// EFFECTS:  creates a REPL object
REPL::REPL(){
    lexer = Lexer();
};


// Start the REPL
// EFFECTS:  starts the REPL
void REPL::start(){
    while(true){
        std::cout << PROMPT;
        std::string input;
        std::getline(std::cin, input);
        if(input == "")
            return;
        lexer = Lexer(input);
        for(Token tok = lexer.nextToken(); tok.type != TokenType::ENDOFFILE; tok = lexer.nextToken()){
            std::cout << "type: " << lexer.TokenTypeToString[tok.type] << ", literal: " << tok.literal << std::endl;
        }
    }
}