#include "rules.h"

//naked subset box
int rule11( struct Sudoku* sud, unsigned int x, unsigned int y ) {
	struct Combinator c;
	SudokuCell cellok;
	SudokuCell subset;

	unsigned int i, j, k;
	unsigned int index[64] = { 0 };
	unsigned int combination[4] = { 0 };
	unsigned int curcell = BOXINDEX( sud, x, y );

	j = 0;

	for( i = 0; i < sud->length; i++ ) {
		if( *sud->cellboxvalue[y][x][i] == 0 && i != curcell ) {
			index[j++] = i;
		}
	}

	if( j <= 3 ) return 0;

	for( i = 2; i < 3; i++ ) {
		Combinator_Initialize( &c, i, index, j );
		combination[i] = curcell;

		while( Combinator_GetNext( &c, combination ) == 0 ) {

			subset = 0;
			for( j = 0; j <= i; j++ ) {
				subset |= combination[j];
				for( k = j + 1; k <= i; k++ ) {
					if( *sud->cellbox[y][x][combination[j]] & *sud->cellbox[y][x][combination[k]] ) {
						cellok |= ( ( 1 << j ) | ( 1 << k ) );
						break;
					}
				}
			}
			if( __popcnt64( cellok ) != i + 1 ) continue;

			for( j = 0; j < sud->length; j++ ) {
				if( ( cellok & ( 1ll << j ) ) == 0 ) {
					*sud->cellbox[y][x][j] &= ( ~subset );
				}
			}

			return 1;
		}

	}

	return 0;
}
