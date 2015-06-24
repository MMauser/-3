#ifndef SOLVER_H
#define SOLVER_H

#define SM_SEQ 0
#define SM_PCELL 1

struct Solver {
	unsigned int ctStrategies;
	unsigned int mode;

	int( **rules )( struct Sudoku* sud, unsigned int x, unsigned int y );

	struct Sudoku* sudoku;
};

int Solver_Initialize( struct Solver* solver, struct Sudoku* sudoku, int strategies, int mode );
int Solver_Solve( struct Solver* solver );

#endif