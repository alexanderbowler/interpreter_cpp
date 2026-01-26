<h1 style="display: flex; align-items: flex-start; justify-content: space-between; margin: 0;">
  <span style="line-height: 1; margin: 0; padding: 0;">Interpreter for Monkey Language</span>
  <img src="README_graphics/ReadmeIcon.png" alt="Interpreter Icon" style="height: 4em; display: block; align-self: flex-start; margin: 0;">
</h1>

## About
This project was created based on the principles in the textbook "Creating an Interpreter in Go". It is an interpreter for Monkey Language, but written in C++. The interpreter  lexes, parses and evaluates Moneky code line by line. The Lexer, Parser, and Evaluator and in the respective files. There is also an extensive Google Tests test suite.

## Table of Contents

## Running the Interpreter
<img src="README_graphics/Compilation.gif" alt="Compilation Demo" style="width: 50%; height: auto;"> \
To run the interpreter follow the below steps: \
Clone the repository\
```git clone https://github.com/alexanderbowler/interpreter_cpp.git```\
Build the interpreter \
```make final``` \
Run the interpreter \
```./interpreter``` \
To quit the interpreter do Ctrl + c

## Available Commands
The interpreter can run a variety of commands: \
Equivalent of `print()` is `puts()` which prints the output in string format and returns null \
Create and bind variables with `let` ie. `let x = 5` \
Evaluate most basic mathematical operations ie. `(5+4) * 8 / 2 + 1 * 3` \
Comparisons of primitive types are supported `1 < 2` `3 != 4` \
Create control flow `if (x < y) { x } else { y }`\
Create functions `let mult = fn(x, y) { return x * y };`\
Arrays and array manipulation `let a = [1, 2, 3]`, `len(a) = 3`, `first(a) = 1`, `push(a, 4)` \
Hashmaps `let h = {"a": 1, "b": 2};`, `h["a"]` \

Demo:\
<img src="README_graphics/Interpreter_demo.gif" alt="Interpreter Demo" style="width: 50%; height: auto;">

## Testing
To compile and run tests do ```cmake -S {source_dir} -B {build_dir}``` ex. ```cmake -S . -B build```\
then run the following, ```cmake --build {build_dir}``` ex. ```cmake --build build```, then run ```cd build && ctest``` from within build_dir

## FAQs
Why C++? \
C++ is a highly performant with many other langauges or compilers written in either C++ or C.
