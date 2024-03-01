/*
    Project Title: Crossword Solver
    Author(s): Logan Fortune
    Date: 21/10/2023
*/

#include "utils.hpp"

class CrosswordGen {
public:
    CrosswordGen(const GridSize gridInfo, const bool manualInput = false);
    // Grid size
    GridSize gridInfo;
    // Path to the different words to be included
    std::vector<std::string> wordsList;
    // Main Solution
    std::vector<WordPlace> sol;
    // Crossword main solver functions
    void run(const bool appCall = false);
    void fillBlank();
    // internal functions
    unsigned prune(std::shared_ptr<Graph> crosswordMaps, const unsigned topN = 3); // [HEURI-3]
    std::vector<WordPlace> getBestSolution(std::shared_ptr<Graph> crosswordMaps, const unsigned minimumTopSolution);
    unsigned getMinimumTopN(std::shared_ptr<Graph> crosswordMaps, std::vector<unsigned>& vectTopN, const unsigned topN = 3);
    void removeBelowTopN(std::shared_ptr<Graph> crosswordMaps, const unsigned minimumTopN);
    void addWordsFromFile();
    bool iteratePosition(const unsigned sizeWord, Position& currentPos);
    void addWordGraph(const std::string word, std::shared_ptr<Graph> crosswordMaps);
    void addWordNode(const std::string word, std::shared_ptr<Graph> crosswordMaps);
    void printGraph(std::shared_ptr<Graph> graph, const unsigned topN = 0);
    void printCrosswordMap(GridMap* node);
    unsigned computeScore(unsigned score, unsigned nbCrossing);
private:
    // [IMPROV-0] English dictionary
    std::vector<std::string> dict; // tdb
    // unique id per node
    static inline std::atomic<int> nextId{0};
};

CrosswordGen::CrosswordGen(const GridSize gridInfo, const bool manualInput){
    this->gridInfo = gridInfo;
    if (manualInput){
        // don't add words from file - manually given by the test
        return;
    }
    addWordsFromFile();
}

void CrosswordGen::addWordsFromFile(){
    // Open words constraints
    readWordsFile("./mylist.txt", wordsList);
#ifdef DEBUGPRINT
    std::cout << "Words that need to be included:" << std::endl;
    for (auto &c: wordsList){
        std::cout << c << std::endl;
    }
#endif
    // [IMPROV-0] Open dictionary
    // readWordsFile("./english-dict/ukenglish.txt", dict);
}

void CrosswordGen::addWordNode(const std::string word, std::shared_ptr<Graph> crosswordMaps){
    // TODO:
    // - [IMPROV-1] add overlapping features => check that an overlap can make a valid new word for the line above and underneath (add options)
    const unsigned sizeWord = word.length();
    GridMap currentNode     = crosswordMaps->currentNode;
    if (sizeWord > gridInfo.width && sizeWord > gridInfo.length){
        return;
    }
    // - [IMPROV-2] Optimized matrix traversal
    for (unsigned x = 0; x < gridInfo.length; x++){
        for (unsigned y  = 0; y < gridInfo.width; y++){
            // Can I place vertically ?
            bool verticalClash   = (sizeWord + y > gridInfo.width);
            // Can I place horizontally ?
            bool horizontalClash = (sizeWord + x  > gridInfo.length);
            // Is there a conflict with a word already positioned ?
            bool horiz_crossing = false;
            int nbHorizCrossing = 0;
            bool vert_crossing  = false;
            int nbVertCrossing  = 0;
            bool clashWithoutCrossing;
            bool clashWithCrossing;
            for (const auto& placedWord: currentNode.wordsMap){
                /*

                            0 1 2 
                        0   * b c
                    |   1
                    |   2
                    v ------> x
                    y

                */
                if (placedWord.is_horizontal) { // placedWord.startPos.y == placedWord.endPos.y
                    assert(placedWord.startPos.y == placedWord.endPos.y);
                    // [IMPROV-1] overlapping on the same line, the line above, the line underneath
                    horizontalClash     |= (isInsideInterval(y, placedWord.startPos.y - 1, placedWord.startPos.y + 1) && (isInsideInterval(x, placedWord.startPos.x, placedWord.endPos.x) || isInsideInterval(x+sizeWord, placedWord.startPos.x, placedWord.endPos.x) || (x < placedWord.startPos.x && (x+sizeWord) > placedWord.endPos.x)));
                    // Intersection clash checking
                    // clash without direct crossing
                    clashWithoutCrossing = ((x == placedWord.startPos.x - 1 || x == placedWord.endPos.x + 1) && isInsideInterval(placedWord.startPos.y, y, y+sizeWord)) || ((placedWord.startPos.y == y - 1 || placedWord.startPos.y == y+sizeWord+1) && isInsideInterval(x, placedWord.startPos.x, placedWord.endPos.x));
                    clashWithCrossing    = (isInsideInterval(x, placedWord.startPos.x, placedWord.endPos.x) && isInsideInterval(placedWord.startPos.y, y, y+sizeWord));
                    vert_crossing        = (clashWithCrossing && placedWord.word[x-placedWord.startPos.x] == word[placedWord.startPos.y-y]);
                    nbVertCrossing      += vert_crossing;
                    verticalClash       |= (clashWithCrossing && placedWord.word[x-placedWord.startPos.x] != word[placedWord.startPos.y-y]) || clashWithoutCrossing;
                } else { // placedWord.startPos.x == placedWord.endPos.x
                    assert(placedWord.startPos.x == placedWord.endPos.x);
                    // [IMPROV-1] overlapping on the same line, the line on the right or the line on the left
                    verticalClash       |= (isInsideInterval(x, placedWord.startPos.x - 1, placedWord.startPos.x + 1) && (isInsideInterval(y, placedWord.startPos.y, placedWord.endPos.y) || isInsideInterval(y+sizeWord, placedWord.startPos.y, placedWord.endPos.y) || (y < placedWord.startPos.y && (y+sizeWord) > placedWord.endPos.y)));
                    // intersection clash checking
                    clashWithoutCrossing = ((y == placedWord.startPos.y - 1 || y == placedWord.endPos.y + 1) && isInsideInterval(placedWord.startPos.x, x, x+sizeWord)) || ((placedWord.startPos.x == x - 1 || placedWord.startPos.x == x+sizeWord+1) && isInsideInterval(y, placedWord.startPos.y, placedWord.endPos.y));
                    clashWithCrossing    = (isInsideInterval(y, placedWord.startPos.y, placedWord.endPos.y) && isInsideInterval(placedWord.startPos.x, x, x+sizeWord));
                    horiz_crossing       = (clashWithCrossing && placedWord.word[y-placedWord.startPos.y] == word[placedWord.startPos.x-x]);
                    nbHorizCrossing     += horiz_crossing;
                    horizontalClash     |= (clashWithCrossing && placedWord.word[y-placedWord.startPos.y] != word[placedWord.startPos.x-x]) || clashWithoutCrossing;
                }
            }
            // Create new graph nodes if no clash
            if (!horizontalClash){
                // add new word in gridmap of the current node
                GridMap newGridMap = currentNode;
                WordPlace newWord{word, {x,y}, {x+sizeWord-1, y}, true};
                newGridMap.wordsMap.push_back(newWord);
                Graph newNode;
                newNode.unique_id   = this->nextId++;
                newNode.currentNode = newGridMap;
                newNode.score       = computeScore(crosswordMaps->score, nbHorizCrossing);
                newNode.parent      = crosswordMaps;
                crosswordMaps->children.push_back(std::make_shared<Graph>(newNode));
            }
            if (!verticalClash){
                // add new word in gridmap of the current node
                GridMap newGridMap = currentNode;
                WordPlace newWord{word, {x,y}, {x, y+sizeWord-1}, false};
                newGridMap.wordsMap.push_back(newWord);
                Graph newNode;
                newNode.unique_id   = this->nextId++;
                newNode.currentNode = newGridMap;
                newNode.score       = computeScore(crosswordMaps->score, nbVertCrossing);
                newNode.parent      = crosswordMaps;
                crosswordMaps->children.push_back(std::make_shared<Graph>(newNode));
            }
        }
    }
}

unsigned CrosswordGen::computeScore(unsigned score, unsigned nbCrossing){
    /* 
    [HEURI-2] 
    In order to create a satisfying crossword, 
    we want to see as many words placed inside the grid 
    as possible but also as many crossings.
    So we created the following score function:
    score = (crossing + words) / (grid length x grid width)
    High score means more interesting crossword!
    There are different way to create a score.
    */
   return score + 1 + nbCrossing;
}

void CrosswordGen::printCrosswordMap(GridMap* node){
    char myArray[gridInfo.length][gridInfo.width];
    for (int y = 0; y <gridInfo.width; y++){
        for (int x = 0; x<gridInfo.length; x++){
            myArray[x][y] = '*';
        }
    }
    for (auto& w: node->wordsMap){
        for(int i=0; i < w.word.size(); i++){
            if(w.is_horizontal) {
                myArray[w.startPos.x+i][w.startPos.y] = w.word[i];
            } else {
                myArray[w.startPos.x][w.startPos.y+i] = w.word[i];
            }
        }
    }
    for (int y = 0; y <gridInfo.width; y++){
        for (int x = 0; x<gridInfo.length; x++){
            std::cout << myArray[x][y] << " ";
        }
        std::cout << std::endl;
    }
}

void CrosswordGen::printGraph(std::shared_ptr<Graph> graph, const unsigned topN){
    if (graph->children.size() == 0){
        if (graph->score >= topN){
            std::cout << "\nSolution Id " << graph->unique_id << " (score=" << graph->score << ") :" << std::endl;
            printCrosswordMap(&graph->currentNode);
        }
    } else {
        // This is not a leaf node
        for(auto node: graph->children){
            printGraph(node, topN);
        }
    }
}

void CrosswordGen::addWordGraph(const std::string word, std::shared_ptr<Graph> crosswordMaps){
    if (crosswordMaps->children.size() == 0){
        // This is a leaf node
        addWordNode(word, crosswordMaps);
    } else {
        // This is not a leaf node
        for(auto node: crosswordMaps->children){
            addWordGraph(word, node);
        }
    }
}

unsigned CrosswordGen::prune(std::shared_ptr<Graph> crosswordMaps, const unsigned topN){
    /*
    [HEURI-3]
    By pruning on each step the graph, we eventually converge to 
    a solution that is locally optimal. We avoid to do too many 
    computation on graphs that seem to not give interesting results.
    */
   std::vector<unsigned> vectTopN;
   unsigned minimumTopN = getMinimumTopN(crosswordMaps, vectTopN, topN);
   removeBelowTopN(crosswordMaps, minimumTopN);
   return minimumTopN;
}

void changeVectorTopN(std::vector<unsigned>& vectTopN, const unsigned score, const unsigned topN = 3){
    bool inserted = false;
    for (unsigned i = 0; i < vectTopN.size(); ++i)
    {
        if(score > vectTopN[i]){
            vectTopN.insert(vectTopN.begin() + i, score);
            inserted = true;
            break;
        }
    }
    if(vectTopN.size() < topN && !inserted){
        vectTopN.push_back(score);
    } else if (vectTopN.size() > topN){
        vectTopN.pop_back();
    }
}

unsigned CrosswordGen::getMinimumTopN(std::shared_ptr<Graph> crosswordMaps, std::vector<unsigned>& vectTopN, const unsigned topN){
    if (crosswordMaps->children.size() == 0){
        // Top N mechanism
        changeVectorTopN(vectTopN, crosswordMaps->score, topN);
    } else {
        // This is not a leaf node
        for(auto node: crosswordMaps->children){
            getMinimumTopN(node, vectTopN, topN);
        }
    }
    return vectTopN.back();
}

void CrosswordGen::removeBelowTopN(std::shared_ptr<Graph> crosswordMaps, const unsigned minimumTopN){
    crosswordMaps->children.erase(std::remove_if(crosswordMaps->children.begin(), crosswordMaps->children.end(), [&minimumTopN](std::shared_ptr<Graph> g) { return (g->children.size() == 0) && (g->score < minimumTopN); }), crosswordMaps->children.end());
    for(auto node: crosswordMaps->children){
        removeBelowTopN(node, minimumTopN);
    }
}

std::vector<WordPlace> CrosswordGen::getBestSolution(std::shared_ptr<Graph> crosswordMaps, const unsigned minimumTopSolution){
    std::vector<WordPlace> sol;
    if (crosswordMaps->children.size() == 0){
        if (crosswordMaps->score >= minimumTopSolution){
            std::cout << "\nSolution Id " << crosswordMaps->unique_id << " (score=" << crosswordMaps->score << ") :" << std::endl;
            printCrosswordMap(&crosswordMaps->currentNode);
            return crosswordMaps->currentNode.wordsMap;
        }
    } else {
        // This is not a leaf node
        for(auto node: crosswordMaps->children){
            return getBestSolution(node, minimumTopSolution);
        }
    }
    return sol;
}

void CrosswordGen::run(const bool appCall) {
    // [HEURI-1] First heuristic: Order words by length
    std::sort(wordsList.begin(), wordsList.end(), [](const std::string& first, const std::string& second){ return first.size() > second.size();});
    // Create Graph
    auto startNode = std::make_shared<Graph>();
    // breadth-first search
    for (const auto& word: wordsList){
        // Add to graph
        addWordGraph(word, startNode); 
    }
    // Process the different results
    if (appCall){
        unsigned top1        = 1;
        unsigned minimumTopN = prune(startNode, top1);
        sol = getBestSolution(startNode, minimumTopN);
    } else {
        // last pruning system
        unsigned top3        = 3;
        unsigned minimumTopN = prune(startNode, top3);
#ifdef DEBUGPRINT
        // print debug
        std::cout << " [PARTIAL-RESULT-INFO] Pruning with Top " << top3 << " minimum score = " << minimumTopN << std::endl;
        printGraph(startNode, minimumTopN);
        std::cout << " ---- " << std::endl;
#endif
    }
}

void CrosswordGen::fillBlank() {
    // TBD
}