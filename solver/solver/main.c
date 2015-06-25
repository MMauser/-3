#include "defaults.h"
#include "parameterset.h"
#include "solver.h"
#include "sudoku.h"
#include "stopwatch.h"

unsigned long long timesum;

#ifdef _WIN32
#if !defined _DEBUG || defined PRINTDEBUG
int run( int arc, wchar_t* argv[] );
int wmain( int argc, wchar_t* argv[] ) {
	unsigned long long i;
	timesum = 0ll;

	for( i = 0; i < 64ll; i++ ) {
		run( argc, argv );
		//wprintf_s( L"\nsolver returned: %i\n", run( argc, argv ) );
	}

	wprintf_s( L"Time avg: (%i msg): %I64d", i, (timesum / i) );
	getchar();
	return 0;
}
int run( int argc, wchar_t* argv[] ) {
#else
int wmain( int argc, wchar_t* argv[] ) {
#endif
#else
int main( int argc, char* argv[] ) {
	//convert to wchar_t
#endif

	struct ParameterSet params;
	struct Solver solver;
	struct Sudoku sudoku;
	struct Stopwatch stopwatch;
	unsigned long long timerun;
	int rc;

	switch( ParameterSet_Parse( &params, argv ) ) {
	case PARAMERROR_NOFILE:
		wprintf_s( L"no sudoku file specified\n" );
		return EXIT_FAILURE;
	case PARAMERROR_NOSOLVER:
		wprintf_s( L"no solver specified\n" );
		return EXIT_FAILURE;
	case PARAMERROR_NOSTRATEGIES:
		wprintf_s( L"no strategies specified\n" );
		return EXIT_FAILURE;
	case PARAMWARNING_NODELIMITER:
		//wprintf_s( L"no delimiter specified, fallback to ' '\n" );
		break;
	case 0:
#ifdef _DEBUG
		wprintf_s( L"_DEBUG:parameter parser succeeded\n" );
#endif
		break;
	}

	if( params.timer != 0 ) Stopwatch_Start( &stopwatch );
	switch( Sudoku_ParseFile( &sudoku, params.filepath, params.delimiter ) ) {
	case SUDOKUERROR_FILE:
		wprintf_s( L"unable to open/read file '%s'\n", params.filepath );
		return EXIT_FAILURE;
	case SUDOKUERROR_GRIDSIZE:
		wprintf_s( L"invalid gridsize\n" );
		return EXIT_FAILURE;
	case SUDOKUERROR_PARSER:
		wprintf_s( L"parsing failed\n" );
		return EXIT_FAILURE;
	case 0:
#ifdef _DEBUG
		wprintf_s( L"_DEBUG:sudoku parser succeeded\n" );
#endif
		break;
	}
	//if( params.timer != 0 ) printf( "time sudokuparser: %I64d\r\n", Stopwatch_GetTime( &stopwatch ) );

#if defined _DEBUG || defined PRINTDEBUG
	wprintf_s( L"_DEBUG: parsed file:\n" );
	Sudoku_Print( &sudoku );
#endif

	if( Solver_Initialize( &solver, &sudoku, params.strategies, params.solvertype ) != 0 ) {
		wprintf_s( L"error initializing solver\n" );
		return EXIT_FAILURE;
	}

	if( params.timer != 0 ) Stopwatch_Start( &stopwatch );
	rc = Solver_Solve( &solver );

	timerun = Stopwatch_GetTime( &stopwatch );
	if( params.timer != 0 ) printf_s( "time solver: %I64d\r\n", timerun );
	timesum += timerun;

#if defined(_DEBUG) || defined(PRINTDEBUG)
	wprintf_s( L"_DEBUG: solve returned: %i\n\ncurrent grid:\n", rc );
	Sudoku_Print( &sudoku );
#endif

	rc = Sudoku_Validate( &sudoku ) != 0;

#if !defined(_DEBUG) || defined(PRINTDEBUG)
	if( rc == 0 ) wprintf_s( L"validation successful.\n" );
	else wprintf_s( L"validation failed.\n" );
#endif
	
	return rc;
}
