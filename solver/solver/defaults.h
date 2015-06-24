#ifndef DEFAULTS_H
#define DEFAULTS_H

#define UNICODE
#define _UNICODE

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#else
//weitere linux includes
#endif

#include <stdlib.h>
#include <stdio.h>

#include <intrin.h>
#endif


//defined to print changed made to grid by rule
//#define PRINTGRIDLOOP
//defined to print result of sudoku validation
#define PRINTVALIDATION
