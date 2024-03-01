# Usage
# make - compile all binary
# make clean - remove all binaries and objects

.PHONY: all clean

CXX = g++
CXXFLAGS=-std=c++20 -O3
DEBUGFLAGS=-g -DDEBUGPRINT

all: crosswordgen_print crosswordgen unittest runtest

debug: CXXFLAGS += ${DEBUGFLAGS}
debug: all

crosswordgen_print:
	@echo "Crossword Gen Compilation..."

crosswordgen: ./src/main.cpp
	@echo "- Compilation main.cpp..."
	${CXX} ${CXXFLAGS} -o crosswordgen -g $<

unittest: ./test/unittest.cpp
	@echo "- Compilation unittest.cpp..."
	${CXX} ${CXXFLAGS} -o unittest -g $<

runtest:
	@echo "- Check compilation..."
	./unittest

clean:
	@echo "Cleaning files."
	rm -rf *.dSYM crosswordgen unittest