#ifndef RULES_H
#define RULES_H

#include "sudoku.h"
#include "combinator.h"

typedef int( *RULE )( struct Sudoku* sud, unsigned int x, unsigned int y );

//--------------------------------------------------------------
//add rules here

int rule0( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule1( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule2( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule3( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule4( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule5( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule6( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule7( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule8( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule9( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule10( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule11( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule12( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule13( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule14( struct Sudoku* sud, unsigned int x, unsigned int y );
int rule15( struct Sudoku* sud, unsigned int x, unsigned int y );


//---------------------------------------------------------------
#endif