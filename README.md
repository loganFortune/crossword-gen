# Crossword Automatic Creation

## Run program
- Add words in `mylist.txt`.
- Run `make` (or `make debug`)
- Run `./crosswordGen width length` to get the result according to the grid size given by `width` and `length`.
- Clean everything: `make clean`

_Falling back to (8 x 8) grid size by default._

#### Hard-coded Limits:
`#define MAXGRIDSIDESIZE 32`: make sure the user is not asking for an impossible task.

#### Code Performance

- Run `make perf` to get performance results of the code.

## Next Version
- _[IMPROV-0]_ Add english dictionary to fill the blank.
- _[IMPROV-1]_ Add overlapping features => check that an overlap can make a valid new word for the line above and underneath (add options).
- _[IMPROV-2]_ Optimized matrix traversal.
- _[IMPROV-3]_ Add parallel Multithreading execution of graph processing.
- _[IMPROV-4]_ Error checking during file reading (multiple errors can happen here).
- _[IMPROV-MINOR]_ Use git hooks for cpp linter and make clean.