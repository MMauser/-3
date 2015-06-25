#include "rules.h"

//prüft ob eine Zelle nur einen möglichen Wert hat
int rule0( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	unsigned int i;

	if( __popcnt64( sud->grid[y][x] ) == 1 ) {
		for( i = 0; i < sud->length; i++ ) {
			if( sud->grid[y][x] & ( 1ll << i ) ) {
				sud->pSetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}
	
	return 0;
}
