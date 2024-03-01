/*
    Project Title: Crossword Solver
    Author(s): Logan Fortune
    Date: 21/10/2023
*/

#include "../src/crosswordgen.hpp"


void test5x5(){
    /*
        Unittest goal:
        - Check that the word 'impossible' cannot be added inside the grid.
        - Check the answers on a simple example.
        - Check that the word 'zizzg' is not inside the best results.
    */
    std::cout << "[test5x5]";
    GridSize gridSize{5,5};
    CrosswordGen CrosswordGen(gridSize, true);
    // Impossible to put in the grid
    CrosswordGen.wordsList.push_back("impossible");
    // possible good match
    CrosswordGen.wordsList.push_back("apple");
    CrosswordGen.wordsList.push_back("pear");
    CrosswordGen.wordsList.push_back("orange");
    CrosswordGen.wordsList.push_back("tacky");
    // Not in the best
    CrosswordGen.wordsList.push_back("zizzg");
    CrosswordGen.run();
    std::cout << " SUCCESS." << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "Crossword Generator Unit Test" << std::endl;
    test5x5();
    return 0;
}