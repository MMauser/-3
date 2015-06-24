#include "sudoku.h"
#include "defaults.h"
#include <math.h>

unsigned char* ReadAllBytes( const wchar_t * file ) {
	FILE *fptr;
	long len;
	unsigned char* retv;

	if( _wfopen_s( &fptr, file, L"r" ) != 0 ) return NULL;

	fseek( fptr, 0, SEEK_END );
	len = ftell( fptr );

	retv = ( unsigned char* ) malloc( len * sizeof( unsigned char ) + sizeof( unsigned char ) );
	if( retv != NULL ) {
		fseek( fptr, 0, SEEK_SET );
		fread( retv, 1, len, fptr );
		retv[len] = L'\0';
	}

	fclose( fptr );
	return retv;
}
int Sudoku_ParseFile( struct Sudoku* sud, const wchar_t* filepath, const wchar_t delimiter ) {
	unsigned char* file;
	unsigned int i, j, rowindex, cellvalue, cellindex;
	int retv;
	SudokuCell mask_possible;

	//read file
	file = ReadAllBytes( filepath );
	if( file == NULL ) return SUDOKUERROR_FILE;

	//clear struct
	memset( sud, 0, sizeof( struct Sudoku ) );
	retv = SUDOKUERROR_PARSER;

	//determine grid size by counting occurences of delimiter in first row
	for( i = 0; file[i] != '\n'; i++ ) {
		if( file[i] == delimiter ) sud->length++;
	}

	//minimum size of grid is 2x2 (1 2 \n3 4 \n)
	if( sud->length < 2 ) {
		retv = SUDOKUERROR_GRIDSIZE;
		goto CLEANUP;
	}
	sud->length_of_box = ( int ) ( 0.5f + sqrt( sud->length ) );
	if( sud->length_of_box * sud->length_of_box != sud->length ) {
		retv = SUDOKUERROR_GRIDSIZE;
		goto CLEANUP;
	}

	mask_possible = 0;
	for( i = 0; i < sud->length; i++ ) {
		mask_possible |= ( 1ll << i );
	}

	//allocate grid
	sud->grid = ( SudokuCell** ) calloc( sud->length, sizeof( SudokuCell* ) );
	if( sud->grid == NULL ) goto CLEANUP;

	sud->cellvalue = ( int** ) calloc( sud->length, sizeof( int* ) );
	if( sud->cellvalue == NULL ) goto CLEANUP;

	//allocate grid rows
	for( i = 0; i < sud->length; i++ ) {
		sud->grid[i] = ( SudokuCell* ) malloc( sud->length * sizeof( SudokuCell ) );
		if( sud->grid[i] == NULL ) goto CLEANUP;
		//alle werte auf möglich setzen
		for( j = 0; j < sud->length; j++ ) sud->grid[i][j] = ( SudokuCell ) mask_possible;

		sud->cellvalue[i] = ( int* ) calloc( sud->length, sizeof( int ) );
	}

	//allocate contains helper
	for( i = 0; i < 3; i++ ) {
		sud->contains[i] = ( unsigned long long* ) calloc( sud->length, sizeof( unsigned long long ) );
		if( sud->contains[i] == NULL ) goto CLEANUP;
	}


	sud->cellbox = ( SudokuCell**** ) calloc( sud->length, sizeof( SudokuCell*** ) );
	if( sud->cellbox == NULL ) goto CLEANUP;

	sud->cellboxvalue = ( unsigned int**** ) calloc( sud->length, sizeof( unsigned int*** ) );
	if( sud->cellboxvalue == NULL ) goto CLEANUP;

	//allocate and set box helper
	for( i = 0; i < sud->length; i++ ) {
		sud->cellbox[i] = ( SudokuCell*** ) calloc( sud->length, sizeof( SudokuCell** ) );
		if( sud->cellbox[i] == NULL ) goto CLEANUP;

		sud->cellboxvalue[i] = ( unsigned int*** ) calloc( sud->length, sizeof( unsigned int** ) );
		if( sud->cellboxvalue[i] == NULL ) goto CLEANUP;

		for( j = 0; j < sud->length; j++ ) {
			//position (0,0) in box-> allocate
			if( j % sud->length_of_box == 0 && i % sud->length_of_box == 0 ) {
				sud->cellbox[i][j] = ( SudokuCell** ) calloc( sud->length, sizeof( SudokuCell* ) );
				if( sud->cellbox[i][j] == NULL ) goto CLEANUP;
				sud->cellboxvalue[i][j] = ( unsigned int** ) calloc( sud->length, sizeof( unsigned int* ) );
				if( sud->cellboxvalue[i][j] == NULL ) goto CLEANUP;
			
			//position (0,x) in box -> copy ptr from above cell)
			} else if( j % sud->length_of_box == 0 ) {
				sud->cellbox[i][j] = sud->cellbox[i - 1][j];
				sud->cellboxvalue[i][j] = sud->cellboxvalue[i - 1][j];
			} else {
				sud->cellbox[i][j] = sud->cellbox[i][j - 1];
				sud->cellboxvalue[i][j] = sud->cellboxvalue[i][j - 1];
			}
		}
	}
	
	//i:y j:x
	for( i = 0; i < sud->length; i += sud->length_of_box ) {
		for( j = 0; j < sud->length; j += sud->length_of_box ) {
			for( cellindex = 0; cellindex < sud->length; cellindex++ ) { 
				sud->cellbox[i][j][cellindex] = &sud->grid[i + cellindex / sud->length_of_box][j + cellindex % sud->length_of_box];
				sud->cellboxvalue[i][j][cellindex] = &sud->cellvalue[i + cellindex / sud->length_of_box][j + cellindex % sud->length_of_box];
			}
		}
	}

	cellvalue = 0;
	cellindex = 0;
	rowindex = 0;

	//parse grid
	for( i = 0; file[i] != '\0'; i++ ) {
		//if current tchar is delimiter, finish cell
		if( file[i] == delimiter ) {
			Sudoku_SetCell( sud, cellindex++, rowindex, cellvalue );
			if( cellindex == sud->length ) {
				cellindex = 0;
				//if no. of rows == no. of cols + 1 end parsing
				if( rowindex == sud->length ) break;
			}
			cellvalue = 0;
			//if current tchar is end of line increment rowindex
		} else if( file[i] == '\n' ) {
			rowindex++;
			//if current tchar is not ctrl tchar, parse as cell content
		} else if( file[i] == '\r' ) {
			continue;
		} else {
			cellvalue *= 10;
			cellvalue += file[i] - '0';
		}
	}



	//skip cleanup
	retv = 0;
	goto END;
CLEANUP:
	//free sudoku if allocated
	if( sud != NULL ) {
		//free sudoku grid if allocated
		if( sud->grid != NULL ) {
			for( i = 0; i < sud->length; i++ ) {
				if( sud->grid[i] != NULL ) free( sud->grid[i] );
			}

			free( sud->grid );
		}
		//free cellvalue grid
		if( sud->cellvalue != NULL ) {
			for( i = 0; i < sud->length; i++ ) {
				if( sud->cellvalue[i] != NULL ) free( sud->cellvalue[i] );
			}
			free( sud->cellvalue );
		}

		if( sud->cellbox != NULL ) {
			for( i = 0; i < sud->length; i += sud->length_of_box ) {
				if( sud->cellbox[i] != NULL ) {
					for( j = 0; j < sud->length; j += sud->length_of_box ) {
						if( sud->cellbox[i][j] != NULL ) free( sud->cellbox[i][j] );
					}
					free( sud->cellbox[i] );
				}
			}
			free( sud->cellbox );
		}

		if( sud->cellboxvalue != NULL ) {
			for( i = 0; i < sud->length; i += sud->length_of_box ) {
				if( sud->cellboxvalue[i] != NULL ) {
					for( j = 0; j < sud->length; j += sud->length_of_box ) {
						if( sud->cellboxvalue[i][j] != NULL ) free( sud->cellboxvalue[i][j] );
					}
					free( sud->cellboxvalue[i] );
				}
			}
			free( sud->cellboxvalue );
		}

		//free contains grid
		for( i = 0; i < 3; i++ ) {
			if( sud->contains[i] != NULL ) free( sud->contains[i] );
		}

		free( sud );
		sud = NULL;
	}
END:
	//free loaded file
	if( file != NULL ) free( file );
	return retv;
}

int Sudoku_Validate( struct Sudoku* sud ) {
	unsigned int i, j;
	unsigned long long contains_mask = 0;

	//erstelle bitmaske 
	//1 bei jedem möglichen bit
	for( i = 0; i < sud->length; i++ ) contains_mask |= ( 1ll << i );

	for( i = 0; i < 3; i++ ) {
		for( j = 0; j < sud->length; j++ ) {
			//wenn eine Nachbarschaft eine Zahl nicht enthält
			//(=> eine andere Zahl doppelt ist)
			//wird das xor mit der '1-Bitmaske' ein Wert ungleich 0 ergeben
			if( ( sud->contains[i][j] ^ contains_mask ) != 0 ) return -1;
		}
	}
	
	return 0;
}

void Sudoku_SetCell( struct Sudoku* sud, unsigned int x, unsigned int y, unsigned int value ) {
	unsigned int i, j;

	if( value == 0 ) return;

	//store value in grid
	sud->cellvalue[y][x] = value;

	//reduce value to shiftvalue (1ll << 0) == 1 
	value--;

	//set all other values to impossible
	sud->grid[y][x] = 0;

	//store value in contains
	sud->contains[CONTAINS_COL][x] |= ( 1ll << value );
	sud->contains[CONTAINS_ROW][y] |= ( 1ll << value );
	sud->contains[CONTAINS_BOX][BOXINDEX( sud, x, y )] |= ( 1ll << value );
	
	//remove other cells possibility for value
	//col / row
	for( i = 0; i < sud->length; i++ ) {
		sud->grid[y][i] &= ~( 1ll << value );
		sud->grid[i][x] &= ~( 1ll << value );
	}

	//box
	//determine box start indices
	x -= ( x % sud->length_of_box );
	y -= ( y % sud->length_of_box );

	//invalidate possibility
	for( j = y; j < y + sud->length_of_box; j++ ) {
		for( i = x; i < x + sud->length_of_box; i++ ) {
			sud->grid[j][i] &= ~( 1ll << value );
		}
	}
}

#define CONSOLE_WIDTH 200
void Sudoku_Print( struct Sudoku* sud ) {
	unsigned int i, j, k;
	printf( "\r\n" );
	for( i = 0; i < sud->length; i++ ) {
		if( i && i % sud->length_of_box == 0 ) {
			for( k = 0; k < CONSOLE_WIDTH; k++ ) printf( "-" );
			printf( "\r\n" );
		}
		for( j = 0; j < sud->length; j++ ) {
			if( j && j % sud->length_of_box == 0 ) printf( "|" );
			if( sud->cellvalue[i][j] != 0 ) {
				if( sud->cellvalue[i][j] < 10 ) {
					printf( "  %i", sud->cellvalue[i][j] );
				} else {
					printf( " %i", sud->cellvalue[i][j] );
				}
			}
			else printf( "   " );
		}
		printf( "\r\n" );
	}
	printf( "\r\n" );
	for( k = 0; k < CONSOLE_WIDTH; k++ ) printf( "--" );
	printf( "\r\n\r\n" );
}
