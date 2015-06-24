#include "rules.h"

//Naked pairs box
int rule5( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i, j, changed;

	if( __popcnt64( sud->grid[y][x] == 2 ) ) {
		for( i = 0; i < sud->length; i++ ) {
			if( ( ( ( sud->grid[y][x] ) ^ ( *sud->cellbox[y][x][i] ) ) == 0 ) && ( &sud->grid[y][x] ) != sud->cellbox[y][x][i] ) {
				changed = 0;
				for( j = 0; j < sud->length; j++ ) {
					changed |= ( ( *sud->cellbox[y][x][j] ) & sud->grid[y][x] );
					*sud->cellbox[y][x][j] &= ( ~( sud->grid[y][x] ) );
				}
				return changed;
			}
		}
	}

	return 0;
}
