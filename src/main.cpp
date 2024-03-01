/*
    Project Title: Crossword Solver
    Author(s): Logan Fortune
    Date: 21/10/2023
*/
#include "crosswordgen.hpp"

int main(int argc, char *argv[])
{
    // configure grid size
    unsigned width, length;
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <width> <length>" << std::endl;
        std::cerr << "Falling back to default grid size {8 x 8}." << std::endl;
    } else {
        std::stringstream ss;

        ss << argv[1];
        ss >> width;
        ss.clear(); // Clear any error flags

        ss << argv[2];
        ss >> length;
    }
    assert(width < MAXGRIDSIDESIZE);
    assert(length < MAXGRIDSIDESIZE);
    GridSize gridSize{width,length};
    CrosswordGen CrosswordGen(gridSize);
    CrosswordGen.run();
    return 0;
}