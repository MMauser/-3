#include "rules.h"

//prüft ob ein Wert innerhalb einer Zeile
//nur in der aktuellen Zelle möglich ist
int rule0( struct Sudoku* sud, unsigned int x, unsigned  int y ) {
	unsigned int i;
	SudokuCell row;
	
	row = 0;
	for( i = 0; i < sud->length; i++ ) {
		if( i != x ) {
			row |= sud->grid[y][i];
		}
	}

	//loop through candidates
	for( i = 0; i < sud->length; i++ ) {

		//wenn kandidat gefunden
		if( ( sud->grid[y][x] & ( 1ll << i ) ) != 0 ) {

			//wenn kandidat an keiner anderen Stelle in Reihe möglich ist
			//Setze Zellwert
			if( ( row & ( 1ll << i ) ) == 0 ) {
				sud->pSetCell( sud, x, y, i + 1 );
				return 1;
			}
		}
	}

	return 0;
}
