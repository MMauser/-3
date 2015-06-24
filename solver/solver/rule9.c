#include "rules.h"

//naked subset column
int rule9 ( struct Sudoku* sud, unsigned int x, unsigned int y )
{
	struct Combinator **c;
	SudokuCell col;
	int i, j = 0;
	int n;
	int index[64];
	int dest[64];
	col = 0;
	memset ( index, 0, 64 );
	for ( i = 0; i < sud->length; i++ )
	{
		if ( sud->cellvalue[i][x] == 0 && i != y )
		{
			index[j++] = i;
		}
	}
	
	for ( i = 2; i < 5; i++ )
	{
		Combinator_Create ( &c, i, index, j );

		while ( !Combinator_GetNext ( c, dest ) )
		{
			for ( n = 0; n < 64; n++ )
			{
				col &= sud->grid[dest[n]][x];
			}
			if ( __popcnt64 ( col ) == i + 1 )
			{
				for ( j = 0; j < sud->length; j++ )
				{
					sud->grid[j][x] &= ~( col );
				}
				
				//Combinator_Free ( c );
				return 1;
			}

		}

	}
		Combinator_Free ( c );
		return 0;
}
