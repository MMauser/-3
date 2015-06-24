#include "stopwatch.h"


void Stopwatch_Start( struct Stopwatch* timer ) {
	LARGE_INTEGER fq;
	QueryPerformanceFrequency( &fq );
	timer->fq = ( double ) fq.QuadPart;
	QueryPerformanceCounter( &timer->li );
}
double Stopwatch_GetTime( struct Stopwatch* timer ) {
	QueryPerformanceCounter( &timer->si );
	return ( timer->si.QuadPart - timer->li.QuadPart ) / timer->fq;
}
