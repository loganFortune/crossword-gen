/*
    Project Title: Crossword Solver
    Author(s): Logan Fortune
    Date: 24/03/2024
*/
#include "crosswordgen.hpp"
#include "../database/db-management.hpp"

int main(int argc, char *argv[])
{
    // configure grid size
    GridSize gridSize{8,8};
    CrosswordGen CrosswordGen(gridSize, APP_INPUT);
    std::vector<DatabaseData> dbFetch;
    getNWords(10, dbFetch);
    // CrosswordGen.run();
    return 0;
}