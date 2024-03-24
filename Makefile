# Usage
# make - compile all binary
# make clean - remove all binaries and objects

.PHONY: all clean

CXX = g++
CXXFLAGS=-std=c++20 -O3 -lsqlite3
DEBUGFLAGS=-g -DDEBUGPRINT

all: crosswordgen_print crosswordgen unittest db-management runtest capp runapp

debug: CXXFLAGS += ${DEBUGFLAGS}
debug: all

app-compile: crosswordgen_print capp runapp

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

db-management: ./src/dbtest.cpp
	@echo "- Compile database management system..."
	${CXX} ${CXXFLAGS} -o dbtest -g $<

capp: ./src/app.cpp
	@echo "- Compile App Algorithm..."
	${CXX} ${CXXFLAGS} -o app -g $<

runtest:
	@echo "- Check compilation..."
	./unittest
	./dbtest

perf-test:
	@echo "- Performance test"
	./perf

runapp:
	@echo "- Running App..."
	./app

clean:
	@echo "Cleaning files."
	rm -rf *.dSYM *.bkp crosswordgen unittest perf dbtest app