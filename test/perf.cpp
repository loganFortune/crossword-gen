/*
    Project Title: Crossword Solver
    Author(s): Logan Fortune
    Date: 21/10/2023
*/
#include <chrono>
#include "../src/crosswordgen.hpp"

unsigned testyxz(const unsigned width, const unsigned length, const std::vector<std::string> words, const bool debugprint){
    /*
        testyxz goal:
        - Create a grid based on a configuration given
        - Solve the crossword problem with some words given
    */
    if(debugprint){
        std::cout << "[testyxz] config: Grid(" << width << "," << length << ") with " << words.size() << " words to fit." << std::endl;
    }
    GridSize gridSize{width,length};
    CrosswordGen CrosswordGen(gridSize, true);
    // Impossible to put in the grid
    for(auto& w: words){
        CrosswordGen.wordsList.push_back(w);
    }
    auto start = std::chrono::high_resolution_clock::now();
    CrosswordGen.run();
    auto stop = std::chrono::high_resolution_clock::now();
    // Calculate duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // Output the time taken
    if (debugprint){
        std::cout << "Time taken by unittest: " << duration.count() << " microseconds" << std::endl;
        std::cout << " SUCCESS." << std::endl;
    }
    return duration.count();
}

int main(int argc, char *argv[])
{
    std::cout << "Crossword Generator Performance Test" << std::endl;
    std::vector<std::string> words;
    // Impossible to put in the grid
    words.push_back("impossible");
    // possible good match
    words.push_back("apple");
    words.push_back("pear");
    words.push_back("orange");
    words.push_back("tacky");
    // Not in the best
    words.push_back("zizzg");
    // More words...
    words.push_back("consitution"); // it won't never consider inside the graph
    words.push_back("hello");
    words.push_back("view");
    words.push_back("earn");
    words.push_back("heal");
    words.push_back("rock");
    words.push_back("make");
    words.push_back("love");
    // Fixed grid size
    unsigned minimumGridSize = 5;
    unsigned maximumGridSize = 9;
    // Fixed nb words
    unsigned minimumNbWords = 5;
    // --- 
    std::cout << "Performance strategy:" << std::endl;
    std::cout << "- Grid size impact for a fix number of words (Min="<< minimumNbWords <<",Max=" << words.size() << ")." << std::endl;
    std::cout << "- Input words impact for a fix grid size (Min="<< minimumGridSize <<",Max=" << maximumGridSize << ")." << std::endl;
    std::vector<std::string>::const_iterator first = words.begin();
    for(int gs = minimumGridSize; gs < maximumGridSize; gs++){
        for(int w = minimumNbWords; w < words.size(); w++){
            std::vector<std::string>::const_iterator last = words.begin() + w;
            std::vector<std::string> newVec(first, last); // create new words vector
            unsigned result = testyxz(gs,gs, newVec, false);
            std::cout << "Grid(" << gs << "," << gs << ") x Words(" << newVec.size() << ") = " << result << std::endl;
        }
    }
    return 0;
}