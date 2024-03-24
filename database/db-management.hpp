/*
    Project Title: Crossword Solver
    Author(s): Logan Fortune
    Date: 17/03/2024
*/
#include <iostream>
#include <cassert>
#include <string>
#include <sqlite3.h> 

struct DatabaseData{
    unsigned id;
    std::string clue;
    std::string answer;
};

int connectionTest(){
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("./database/clue-answer.db", &db);

   if(rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    return 1;
   } else {
    std::cout << "Opened database successfully." << std::endl;
   }
   sqlite3_close(db);
   return 0;
}

void getNWords(const unsigned nbWords, std::vector<DatabaseData>& dbFetch){
    // todo
}