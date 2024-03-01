/*
    Project Title: Crossword Solver
    Author(s): Logan Fortune
    Date: 21/10/2023
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <atomic>
#include <sstream>
#include <cassert>

#define MAXGRIDSIDESIZE 32

struct GridSize{
    unsigned width, length;
};

struct Position{
    unsigned x, y;
};

struct WordPlace{
    std::string word;
    Position startPos;
    Position endPos;
    bool is_horizontal;
};

struct GridMap{
    
    std::vector<WordPlace> wordsMap;

};

struct Graph {
    unsigned unique_id;
    float score = 0.0;
    GridMap currentNode;
    std::shared_ptr<Graph> parent;
    std::vector<std::shared_ptr<Graph>> children;
};

bool isInsideInterval(int val, int startInterval, int endInterval){
    return (val >= startInterval && val <= endInterval);
}

void readWordsFile(const std::string pathWordsList, std::vector<std::string> &wordsList) {
    try{
        std::ifstream infile(pathWordsList);
        std::string   currentString;
        if (infile.is_open()) {
            while (infile.good()) {
                // The >> operator reads a string only until 
                // it encounters a white space character 
                // (such as a space or line break).
                infile >> currentString;
                wordsList.push_back(currentString);
            }
        }
    } catch (...) {
        std::cout << "[readWordsFile] Something went wrong reading the following file: " << pathWordsList << std::endl;
    }
}

//===========================================================================
// Overloading cout << operator.  Since cout is actually an ostream variable,
// our return-by-reference and call-by-reference uses ostream and passes
// whatever struct we're working with as a const (has to be)
std::ostream& operator<<(std::ostream& os, const Position& pos)
{
    os << "(x=" << pos.x << ",y=" << pos.y << ")" << std::endl;

    return os;
}