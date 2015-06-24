#include "rules.h"
//Prueft hidden pairs in Spalten
int rule6 ( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j;
	SudokuCell candidate, neighbourhood;

	for( i = 0; i < sud->length; i++ ) {
		candidate = ( sud->grid[y][x] ) & ( sud->grid[i][x] );
		if( __popcnt64( candidate ) == 2 && i != y ) {
			if( ( ( __popcnt64( sud->grid[y][x] ) != 2ll ) || ( __popcnt64( sud->grid[i][x] != 2ll ) ) ) != 0 ) {
				neighbourhood = 0;
				for( j = 0; j < sud->length; j++ ) {
					if( j != i && j != y ) neighbourhood |= sud->grid[j][x];
				}

				if( ( candidate & neighbourhood ) == 0 ) {
					sud->grid[i][x] = candidate;
					sud->grid[y][x] = candidate;
					return 1;
				}
			}
			return 0;
		}
	}
	return 0;
}