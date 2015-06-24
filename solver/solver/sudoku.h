#ifndef SUDOKU_H
#define SUDOKU_H

#include "defaults.h"

//fix!
//do not change.
#define CELL_POSSIBLE 1
#define CELL_IMPOSSIBLE 0

#define CONTAINS_ROW 0
#define CONTAINS_COL 1
#define CONTAINS_BOX 2

#define SUDOKUERROR_FILE (-1)
#define SUDOKUERROR_GRIDSIZE (-2)
#define SUDOKUERROR_PARSER (-3)

#define BOXINDEX(sud, x, y) ( ( y / sud->length_of_box ) * sud->length_of_box + x / sud->length_of_box )

typedef unsigned long long SudokuCell;

struct Sudoku {
	//enthält Möglichkeiten für Zelle
	//Bit an Position (1 << Zahl) ist 1 wenn möglich, 0 wenn nicht
	SudokuCell** grid;

	//enthält aktuelle Zahl der Zelle
	//0: leere Zelle
	int** cellvalue;


	//enthält die Möglichkeiten aus der zur Zelle gehörigen Box
	//die Werte sind doppelt zu dem Feld cellvalue
	//->dient zum einfachen Zugriff auf die zugehörige Box
	//bsp 9x9:
	//Box:
	//[... 0 1 2 ...]
	//[... 3 4 5 ...]
	//[... 6 7 8 ...]
	//=> cellboxvalue[y][x][0-9] = [0 1 2 3 4 5 6 7 8]
	SudokuCell**** cellbox;
	
	//enthält die Werte aus der zur Zelle gehörigen Box
	//die Werte sind doppelt zu dem Feld cellvalue
	//->dient zum einfachen Zugriff auf die zugehörige Box
	//bsp 9x9:
	//Box:
	//[... 0 1 2 ...]
	//[... 3 4 5 ...]
	//[... 6 7 8 ...]
	//=> cellboxvalue[y][x][0-9] = [0 1 2 3 4 5 6 7 8]
	unsigned int**** cellboxvalue;

	unsigned int length;
	unsigned int length_of_box;

	//64Bit Contains Bitmaske
	//bis zu 49*49 sudoku
	//Bitwerte:
	//1: Zahl ist enthalten
	//0: Zahl ist nicht enthalten
	unsigned long long* contains[3]; 

	//funktion zum setzen einer Zelle
	//wird vom Solver gesetzt
	//abhängig vom parallelisierungsmode
	void( *pSetCell )( struct Sudoku* sudoku, unsigned int x, unsigned int y, unsigned int value );
};

//liest eine datei ein und erstellt entsprechende sudoku instanz
//Nach jedem Zellwert muss ein delimiter stehen
//Leere Zellen können als '0' oder '' dargestellt werden.
//Besipiel:
//d: delimiter
//0: leere Zelle
//\n: Zeilenumbruch
//0d1d\n
//1d2d\n
int Sudoku_ParseFile( struct Sudoku* dest, const wchar_t* file, const wchar_t delimiter );


//Diese Funktion sollte nicht direkt aufgerufen werden
//die Sudoku instanz enthält eine Funktion
//welche vom solver entsprechend der parallelisierung gewählt wird
//setzt eine Zelle im Sudoku auf den angegeben Wert
//x,y: koordinate der zu setzenden Zelle
//value: wert die die Zelle enthalten soll
//ist value = 0 werden keine Änderungen vorgenommen
void Sudoku_SetCell( struct Sudoku* sud, unsigned int x, unsigned int y, unsigned int value );

//stellt das Sudoku auf der Konsole dar
void Sudoku_Print( struct Sudoku* sud );

//prüft ob das Sudoku gültig ist
//0: gültig
//-1: nicht gültig
int Sudoku_Validate( struct Sudoku* sud );

#endif