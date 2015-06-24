#ifndef TIMER_H
#define TIMER_H

#include "defaults.h"

struct Stopwatch {
	LARGE_INTEGER si, li;
	double fq;
};

void Stopwatch_Start( struct Stopwatch* timer );
double Stopwatch_GetTime( struct Stopwatch* timer );

#endif