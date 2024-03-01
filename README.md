# Crossword Automatic Creation

## Run program
- Add words in `mylist.txt`.
- Run `make` (or `make debug`)
- Run `./crosswordGen width length` to get the result according to the grid size given by `width` and `length`.
- Clean everything: `make clean`

_Falling back to (8 x 8) grid size by default._

#### Hard-coded Limits:
`#define MAXGRIDSIDESIZE 32`: make sure the user is not asking for an impossible task.

## Next Version
- [IMPROV-0] Add english dictionary to fill the blank.
- [IMPROV-1] Add overlapping features => check that an overlap can make a valid new word for the line above and underneath (add options).
- [IMPROV-2] Optimized matrix traversal
