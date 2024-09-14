# Interpreter for Monkey
## About
A basic interpreter that lexes, parses and evaluates Moneky code line by line. The Lexer, Parser, and Evaluator and in the respective files.

## Running the Interpreter
To run the REPL compile and run repl.cpp


### Testing
To compile and run tests do ```cmake -S {source_dir} -B {build_dir}``` ex. ```cmake -S . -B build```
then run the following, ```cmake --build {build_dir}```, then run ```ctest``` from within build_dir
