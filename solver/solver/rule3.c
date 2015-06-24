#include "rules.h"

//naked pair row
int rule3( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j, changed;
	if( __popcnt64( sud->grid[y][x] ) != 2 ) return 0;

	for( i = 0; i < sud->length; i++ ) {
		if( i == x ) continue;
		if( (sud->grid[y][i] ^ sud->grid[y][x]) == 0 ) {
			changed = 0;
			for( j = 0; j < sud->length; j++ ) {
				if( j != i && j != x ) {
					changed |= ( sud->grid[y][j] & sud->grid[y][x] );
					sud->grid[y][j] &= ( ~( sud->grid[y][x] ) );
				}
			}

			return changed;
		}
	}

	return 0;
}
