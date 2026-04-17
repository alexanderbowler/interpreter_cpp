<h1 style="display: flex; align-items: flex-start; justify-content: space-between; margin: 0; gap: 1rem;">
  <span style="line-height: 1; margin: 0; padding: 0; flex: 1 1 auto;">Interpreter for Monkey Language</span>
  <img src="README_graphics/ReadmeIcon.png" alt="Interpreter Icon" style="height: 4em; display: block; align-self: flex-start; margin: 0;">
</h1>

## About

This project was created based on the principles in the textbook "Writing an Interpreter in Go". It is an interpreter for the Monkey language, but written in C++. The interpreter lexes, parses, and evaluates Monkey code line by line. The Lexer, Parser, and Evaluator are implemented in their respective files. There is also an extensive GoogleTest test suite.

## Table of Contents

- [About](#about)
- [How the Interpreter Works](#how-the-interpreter-works)
- [Running the Interpreter](#running-the-interpreter)
- [Available Commands](#available-commands)
- [Testing](#testing)
- [FAQs](#faqs)

## How the Interpreter Works

The interpreter runs each line of Monkey code through three main stages:

1. The lexer reads the source text and converts it into tokens, such as identifiers, numbers, operators, keywords, brackets, and strings.
2. The parser consumes those tokens and builds an abstract syntax tree (AST) that represents the structure of the program.
3. The evaluator walks the AST, resolves variables through the environment, evaluates expressions, applies functions, and returns the resulting object.

The diagram below shows how source input moves through those stages before producing an evaluated result.

<img src="README_graphics/InterpreterDiagram.png" alt="Interpreter Diagram" width="900">

## Running the Interpreter

To run the interpreter, follow these steps:

Clone the repository:

```bash
git clone https://github.com/alexanderbowler/interpreter_cpp.git
```

Build the interpreter:

```bash
make final
```

Run the interpreter:

```bash
./interpreter
```

To quit the interpreter, press `Ctrl+C`.

The demo below shows the clone, build, and run flow:

<img src="README_graphics/Compilation.gif" alt="Compilation Demo" width="900">

## Available Commands

The interpreter can run a variety of commands:

- The equivalent of `print()` is `puts()`, which prints the output in string format and returns `null`.
- Create and bind variables with `let`, e.g., `let x = 5`.
- Evaluate most basic mathematical operations, e.g., `(5 + 4) * 8 / 2 + 1 * 3`.
- Compare primitive types, e.g., `1 < 2` and `3 != 4`.
- Create control flow with `if (x < y) { x } else { y }`.
- Create functions, e.g., `let mult = fn(x, y) { return x * y };`.
- Create and use arrays, e.g., `let a = [1, 2, 3]`, `len(a)`, `first(a)`, and `push(a, 4)`.
- Create and index hash maps, e.g., `let h = {"a": 1, "b": 2};` and `h["a"]`.

The demo below shows several supported Monkey language features running in the interpreter:

<img src="README_graphics/Interpreter_demo.gif" alt="Interpreter Demo" width="900">

## Testing

To compile and run the tests, configure the build directory:

```bash
cmake -S . -B build
```

Build the test target:

```bash
cmake --build build
```

Run the test suite from the build directory:

```bash
cd build
ctest
```

## FAQs

### Why C++?

C++ is highly performant, and many languages and compilers are written in C or C++.

### What is the difference between an interpreted vs. compiled language?

A compiled language translates the entire program into machine code before it runs. This usually creates an executable file and can make programs run faster. An interpreted language runs code live by reading and executing it step by step through an interpreter at runtime. This often makes testing and debugging easier, but execution can be slower.