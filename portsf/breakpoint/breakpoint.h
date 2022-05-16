#include <stdio.h> 
#include <stdlib.h>


struct BreakPoint {
	double time; 
	double value; 
}; 

typedef struct BreakPoint breakpoint_t; 

struct TickPoint {
	unsigned long tickpoint; 
	double value; 
}; 

typedef struct TickPoint tickpoint_t; 

float larger(float first, float second); 
breakpoint_t maxpoint(const breakpoint_t* points, long npoints); 
breakpoint_t* getBreakpoints(FILE *fp, long* psize); 




