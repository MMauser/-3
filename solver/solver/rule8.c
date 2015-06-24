#include "rules.h"

//Hidden Pairs in Boxen
int rule8( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell candidate, neighbourhood;

	for( i = 0; i < sud->length; i++ ) {
		candidate = ( sud->grid[y][x] ) & ( *sud->cellbox[y][x][i] );
		if( ( __popcnt64( candidate ) == 2ll ) && ( &sud->grid[y][x] != sud->cellbox[y][x][i] ) ) {
			if( __popcnt64( sud->grid[y][x] ) != 2ll || __popcnt64( *sud->cellbox[y][x][i] ) != 2ll ) {
				neighbourhood = 0;
				for( j = 0; j < sud->length; j++ ) {
					if( sud->cellbox[y][x][j] != &sud->grid[y][x] && j != i ) neighbourhood |= *sud->cellbox[y][x][j];
				}

				if( ( candidate & neighbourhood ) == 0 ) {
					*sud->cellbox[y][x][i] = candidate;
					sud->grid[y][x] = candidate;
					return 1;
				}
			}
			return 0;
		}
	}
	return 0;
}