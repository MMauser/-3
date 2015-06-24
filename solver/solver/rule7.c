#include "rules.h"

//Hidden pairs in Reihen
int rule7( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell candidate, neighbourhood;

	for( i = 0; i < sud->length; i++ ) {
		candidate = ( sud->grid[y][x] ) & ( sud->grid[y][i] );
		if( __popcnt64( candidate ) == 2ll && i != x ) {
			if( ( __popcnt64( sud->grid[y][x] ) != 2ll ) || ( __popcnt64( sud->grid[y][i] ) != 2ll ) ) {
				neighbourhood = 0;
				for( j = 0; j < sud->length; j++ ) {
					if( j != i && j != x ) neighbourhood |= sud->grid[y][j];
				}

				if( ( candidate & neighbourhood ) == 0 ) {
					sud->grid[y][i] = candidate;
					sud->grid[y][x] = candidate;
					return 1;
				}
			}
			return 0;
		}
	}
	return 0;
}