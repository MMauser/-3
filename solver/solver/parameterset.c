#include "parameterset.h"
#include "defaults.h"

int ParameterSet_Parse( struct ParameterSet* set , wchar_t** argv ) {
	unsigned int i, j;

	if( set == NULL ) return PARAMERROR_INVALIDARG;

	set->solvertype = ( unsigned int ) -1;
	set->strategies = 0;
	set->filepath[0] = 0;
	set->delimiter = 0;
	set->timer = 0;

	i = ( unsigned int ) -1;
	while( argv[++i] != NULL ) {
		if( argv[i][0] != L'-' ) continue;

		//Parameter:
		//strategies:
		// -st[0-9]
		//parallelization:
		// -par=[0-9]
		//sudoku filepath:
		// -sud={GUID}.tmp
		//trennzeichen: (optional)
		//-del=[wchar_t]

		switch( argv[i][2] ) {
		case L'a': //par
			set->solvertype = argv[i][5] - L'0';
			break;
		case L'u': //sud
			j = 5;
			while( argv[i][j] ) {
				set->filepath[j - 5] = argv[i][j];
				j++;
			}
			set->filepath[j - 5] = 0;
			break;
		case L'd':
			set->delimiter = argv[i][5];
			break;
		case L'i':
			set->timer = 1;
			break;
		case L't': //st
			j = argv[i][3] - L'0';
			if( argv[i][4] >= L'0' && argv[i][4] <= L'9' ) { //2. Stelle
				j *= 10;
				j += argv[i][4] - _T( '0' );
			}

			set->strategies |= ( 1 << j );
			break;
		default:
			wprintf( L"Unknown parameter: '%s'\nfound at position: %i", argv[i], i );
			break;
		}
	}

	if( set->filepath[0] == 0 ) return PARAMERROR_NOFILE;
	if( set->solvertype == ( unsigned int ) -1 ) return PARAMERROR_NOSOLVER;
	if( set->strategies == 0 ) return PARAMERROR_NOSTRATEGIES;
	if(set->delimiter == 0 ) {
		set->delimiter = L' ';
		return PARAMWARNING_NODELIMITER;
	}

	return 0;
}
