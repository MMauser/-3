#include "Combinator.h"
#include <string>
#include <algorithm>

struct PCombinator {
	struct Combinator c;
	std::string bitmask; // K leading 1's
	int n;
};
//C = Combiation k = Größe Subset values = Kantidatenliste N = Sudoku Größe
int Combinator_Create( struct Combinator** c, int k, int* values, int n ) {
	*c = ( Combinator* ) malloc( sizeof( PCombinator ) );

	( *c )->values = values;
	( ( PCombinator* ) c )->bitmask = std::string( k, 1 );
	( ( PCombinator* ) c )->bitmask.resize( n, 0 );
	( ( PCombinator* ) c )->n = n;

	return 0;
}
int Combinator_GetNext(struct Combinator* c, int* dest) {
	for( int i = 0, cd = 0; i < ( ( PCombinator* ) c )->n; i++ ) {
		if( ( ( PCombinator* ) c )->bitmask[i] ) dest[cd++] = c->values[i];
	}

	return !std::prev_permutation( ( ( PCombinator* ) c )->bitmask.begin(), ( ( PCombinator* ) c )->bitmask.end() );	
}
void Combinator_Free( struct Combinator* c ) {
	free( c );
}
