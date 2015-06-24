#include "rules.h"

//prüft ob ein Wert innerhalb einer Spalte
//nur in der aktuellen Zelle möglich ist
int rule1( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	unsigned int i;
	SudokuCell col;

	col = 0;
	for( i = 0; i < sud->length; i++ ) {
		if( i != y ) {
			col |= sud->grid[i][x];
		}
	}

	//loop through candidates
	for( i = 0; i < sud->length; i++ ) {

		//wenn kandidat gefunden
		if( ( sud->grid[y][x] & ( 1ll << i ) ) != 0 ) {

			//wenn kandidat an keiner anderen Stelle in Spalte möglich ist
			//Setze Zellwert
			if( ( col & ( 1ll << i ) ) == 0 ) {
				sud->pSetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}
