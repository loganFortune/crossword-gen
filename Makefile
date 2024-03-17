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

perf: crosswordgen_print crosswordgen performance perf-test

crosswordgen_print:
	@echo "Crossword Gen Compilation..."

crosswordgen: ./src/main.cpp
	@echo "- Compilation main.cpp..."
	${CXX} ${CXXFLAGS} -o crosswordgen -g $<

unittest: ./test/unittest.cpp
	@echo "- Compilation unittest.cpp..."
	${CXX} ${CXXFLAGS} -o unittest -g $<

performance: ./test/perf.cpp
	@echo "- Compilation perf.cpp..."
	${CXX} ${CXXFLAGS} -o perf -g $<

runtest:
	@echo "- Check compilation..."
	./unittest

perf-test:
	@echo "- Performance test"
	./perf

clean:
	@echo "Cleaning files."
	rm -rf *.dSYM *.bkp crosswordgen unittest perf