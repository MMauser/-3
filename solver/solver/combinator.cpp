#include "Combinator.h"
#include <string>
#include <algorithm>

//C = Combiation k = Größe Subset values = Kantidatenliste N = Sudoku Größe
int Combinator_Initialize( struct Combinator* c, int k, int* values, int n ) {

	c->values = values;
	c->bitmask = std::string( k, 1 );
	c->bitmask.resize( n, 0 );
	c->n = n;

	return 0;
}
int Combinator_GetNext(struct Combinator* c, int* dest) {
	for( int i = 0, cd = 0; i < c->n; i++ ) {
		if( c->bitmask[i] ) dest[cd++] = c->values[i];
	}

	return !std::prev_permutation( c->bitmask.begin(), c->bitmask.end() );	
}
