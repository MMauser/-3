#ifndef TIMER_H
#define TIMER_H

#include "defaults.h"

//#define STOPWATCH_TIME

struct Stopwatch {
	LARGE_INTEGER si, li;
	double fq;
};

void Stopwatch_Start( struct Stopwatch* timer );

#ifdef STOPWATCH_TIME
double Stopwatch_GetTime( struct Stopwatch* timer );
#else
unsigned long long Stopwatch_GetTime( struct Stopwatch* timer );
#endif

#endif