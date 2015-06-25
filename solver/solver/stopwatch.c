#include "stopwatch.h"


void Stopwatch_Start( struct Stopwatch* timer ) {
#ifdef STOPWATCH_TIME
	LARGE_INTEGER fq;
	QueryPerformanceFrequency( &fq );
	timer->fq = ( double ) fq.QuadPart;
#endif
	QueryPerformanceCounter( &timer->li );
}

#ifdef STOPWATCH_TIME
double Stopwatch_GetTime( struct Stopwatch* timer ) {
	QueryPerformanceCounter( &timer->si );
	return ( timer->si.QuadPart - timer->li.QuadPart ) / timer->fq;
}
#else
unsigned long long Stopwatch_GetTime( struct Stopwatch* timer ) {
	QueryPerformanceCounter( &timer->si );
	return ( timer->si.QuadPart - timer->li.QuadPart );
}
#endif

