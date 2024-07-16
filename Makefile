# Makefile for interpreter project to run REPL

# Desired executable name
EXECUTABLE = interpreter

# The following line looks for a project's main() in files named project*.cpp,
# executable.cpp (substituted from EXECUTABLE above), or lastly checks for main.cpp
PROJECTFILE = $(or $(wildcard project*.cpp $(EXECUTABLE).cpp), main.cpp)

# disable built in compile rules
.SUFFIXES:

# Compiler to use
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic 

# rule for making object files from .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

# list of test source files
TESTSOURCES = $(wildcard *test*.cpp)

# list of sources used in project
SOURCES     = $(wildcard *.cpp)
SOURCES     := $(filter-out $(TESTSOURCES), $(SOURCES))

# list of objects used in project
OBJECTS     = $(SOURCES:%.cpp=%.o)

# make debug - will compile sources with $(CXXFLAGS) -g3 and -fsanitize
#              flags also defines DEBUG and _GLIBCXX_DEBUG
debug: CXXFLAGS += -g3 -DDEBUG -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG
debug:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)_debug
.PHONY: debug

# make final - will compile sources with $(CXXFLAGS) -g3 and -fsanitize
#              flags also defines DEBUG and _GLIBCXX_DEBUG
final: CXXFLAGS += -o3 
final:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)
.PHONY: debug

# make valgrind - will compile sources with $(CXXFLAGS) -g3 suitable for
#                 CAEN or WSL (DOES NOT WORK ON MACOS).
valgrind: CXXFLAGS += -g3
valgrind:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)_valgrind
.PHONY: valgrind