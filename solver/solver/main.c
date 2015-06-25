#include "defaults.h"
#include "parameterset.h"
#include "solver.h"
#include "sudoku.h"
#include "stopwatch.h"

unsigned long long timesum;

#ifdef SUDOKU_UI

static struct Sudoku* gsud;
HWND hWnd;

static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
DWORD WINAPI hWindow( struct Sudoku* sud ) {
	ATOM cid = 0;
	MSG msg = { 0 };
	WNDCLASS wndclass = { 0 };
	HINSTANCE hInstance = GetModuleHandle( 0 );

	gsud = sud;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = ( HICON ) LoadIcon( 0, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = CreateSolidBrush( RGB( 240, 240, 240 ) );
	wndclass.lpszClassName = _T( "MyWindowClass" );

	cid = RegisterClass( &wndclass );
	if( cid == 0 ) {
		MessageBox( NULL, _T( "RegisterClass has failed" ), NULL, MB_OK | MB_ICONERROR );
		DestroyIcon( wndclass.hIcon );
		return EXIT_FAILURE;
	}

	hWnd = CreateWindow( ( LPTSTR ) cid, _T( "Ohne Konservierungsstoffe" ), WS_OVERLAPPEDWINDOW, 0, 0, 720, 720, NULL, NULL, hInstance, NULL );

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	while( GetMessage( &msg, NULL, 0, 0 ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	UnregisterClass( ( LPTSTR ) cid, hInstance );

	return ( int ) msg.wParam;
}


void onPaint( WPARAM wParam, LPARAM lParam ) {
	HPEN hBoxLine = CreatePen( PS_SOLID, 2, 0 );
	PAINTSTRUCT ps;
	RECT rcClient;
	HDC hdc = BeginPaint( hWnd, &ps );
	wchar_t buffer[512];
	HANDLE hOldPen;
	HFONT f = CreateFont( 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"consolas" );

	HFONT hOldFont = SelectObject( hdc, f );

	GetClientRect( hWnd, &rcClient );

	int width = rcClient.right / gsud->length;
	int height = rcClient.bottom / gsud->length;

	SetBkMode( hdc, TRANSPARENT );

	for( int i = 0; i < gsud->length; i++ ) {
		for( int x = 0; x < gsud->length; x++ ) {
			int iy = 0;
			int ix = 0;
			if( gsud->cellvalue[i][x] != 0 ) {
				SetTextColor( hdc, RGB( 0, 150, 0 ) );
				wsprintfW( buffer, L"%i", gsud->cellvalue[i][x] );
				TextOut( hdc, x * width + width / 2 - 5, i * height + height / 2 - 5, buffer, lstrlenW( buffer ) );
			} else {

				for( int p = 0; p < gsud->length; p++ ) {


					if( p != 0 && p % gsud->length_of_box == 0 ) {
						iy += height / gsud->length_of_box;
						ix = 0;
					} else if( p ) ix += width / gsud->length_of_box;

					if( gsud->grid[i][x] & ( 1 << p ) ) {
						SetTextColor( hdc, 0 );
					} else SetTextColor( hdc, RGB( 200, 200, 200 ) );

					wsprintfW( buffer, L"%i", p + 1 );
					TextOutW( hdc, x * width + ix, i * height + iy, buffer, lstrlenW( buffer ) );
				}
			}
		
		}

		if( i != 0 && i % gsud->length_of_box == 0 ) {
			hOldPen = SelectObject( hdc, hBoxLine );
			MoveToEx( hdc, 0, i * height, NULL );
			LineTo( hdc, rcClient.right, i * height );
			MoveToEx( hdc, i * width, 0, NULL );
			LineTo( hdc, i * width, rcClient.bottom );
			SelectObject( hdc, hOldPen );
		} else {
			MoveToEx( hdc, 0, i * height, NULL );
			LineTo( hdc, rcClient.right, i * height );
			MoveToEx( hdc, i * width, 0, NULL );
			LineTo( hdc, i * width, rcClient.bottom );
		}
	}

	SelectObject( hdc, hOldFont );

	DeleteObject( f );
	DeleteObject( hBoxLine );
	EndPaint( hWnd, &ps );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	static HANDLE hProcess;

	switch( msg ) {
	case WM_CREATE:
		SetTimer( hWnd, 0, 1000, NULL );
		return 0;
	case WM_TIMER:
		InvalidateRect( hWnd, NULL, TRUE );
		return 0;
	case WM_PAINT:
		onPaint( wParam, lParam );
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void ForceRedraw() {
	onPaint( NULL, NULL );
}

#endif




#ifdef _WIN32
#if !defined _DEBUG || defined PRINTDEBUG
int run( int arc, wchar_t* argv[] );
int wmain( int argc, wchar_t* argv[] ) {
	unsigned long long i;
	int rc;
	timesum = 0ll;

	for( i = 0; i < 64ll; i++ ) {
		rc = run( argc, argv );
		//wprintf_s( L"\nsolver returned: %i\n", run( argc, argv ) );
	}

	wprintf_s( L"Time avg: (%i runs): %I64d, retv:%i", i, (timesum / i), rc );
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
#ifdef SUDOKU_UI
		CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) &hWindow, ( LPVOID ) &sudoku, 0, NULL );
#endif
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

#if defined(_DEBUG) || defined(PRINTDEBUG)
	if( rc == 0 ) wprintf_s( L"validation successful.\n" );
	else wprintf_s( L"validation failed.\n" );
#if defined SUDOKU_UI && defined _DEBUG
	getchar();
#endif
#endif
	

	return rc;
}
