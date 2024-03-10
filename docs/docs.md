# Crossword Automatic Generation Documentation

This file aims to give some details about the internal mechanism (or high-level strategy) of the repo.

## Internal mechanism

TBD - Insert high-level diagram of the strategy.

## Internal database

_Database Name_:
`
sqlite> .databases
main: ./CrosswordSolver/database/questions-answers.db r/w
`
_Database Table_:

CREATE TABLE CLUEANSWER (
    CAId INTEGER PRIMARY KEY, -- Automatically increments
    Clue TEXT NOT NULL,
    Answer TEXT -- Removed VARCHAR(32) for compatibility, but TEXT works fine
);

_Database Debug_:

Open the database:
- Run `cd database && sqlite3 clue-answer.db`.

Get the different commands:
- Run `.help`.

Get some specific information about the tables:
- Run `.databases`, `.tables`, `.schema CLUEANSWER`.
- Run `SELECT * FROM CLUEANSWER;`.

Insert new data:
- Run `INSERT INTO CLUEANSWER(Clue, Answer) VALUES('He is best known for the motoring programmes Top Gear.', 'Clarkson');`

## Performance

#### Strategy 

- Grid size impact for a fix number of words (Min=5,Max=14).
- Input words impact for a fix grid size (Min=5,Max=9).

_Important consideration_:
- The average size of the words matters as well. 
- In the performance test, most of the words are roughly 5 characters long.

#### Last Performance Results:

Grid(5,5) x Words(5) = 118
Grid(5,5) x Words(6) = 98
Grid(5,5) x Words(7) = 95
Grid(5,5) x Words(8) = 93
Grid(5,5) x Words(9) = 113
Grid(5,5) x Words(10) = 129
Grid(5,5) x Words(11) = 151
Grid(5,5) x Words(12) = 162
Grid(5,5) x Words(13) = 181
Grid(6,6) x Words(5) = 710
Grid(6,6) x Words(6) = 941
Grid(6,6) x Words(7) = 929
Grid(6,6) x Words(8) = 1075
Grid(6,6) x Words(9) = 1506
Grid(6,6) x Words(10) = 1579
Grid(6,6) x Words(11) = 1614
Grid(6,6) x Words(12) = 1963
Grid(6,6) x Words(13) = 1264
Grid(7,7) x Words(5) = 6738
Grid(7,7) x Words(6) = 12810
Grid(7,7) x Words(7) = 12854
Grid(7,7) x Words(8) = 17973
Grid(7,7) x Words(9) = 25532
Grid(7,7) x Words(10) = 32480
Grid(7,7) x Words(11) = 40104
Grid(7,7) x Words(12) = 49904
Grid(7,7) x Words(13) = 57629
Grid(8,8) x Words(5) = 123498
Grid(8,8) x Words(6) = 426340
Grid(8,8) x Words(7) = 443009
Grid(8,8) x Words(8) = 817653
Grid(8,8) x Words(9) = 1300524
Grid(8,8) x Words(10) = 1776765
Grid(8,8) x Words(11) = 2277623
Grid(8,8) x Words(12) = 2748631
Grid(8,8) x Words(13) = 3261543

#### Comments

We can notice a quite poor performance of the tool for grids size over 8x8.