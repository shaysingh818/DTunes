#include "breakpoint.h"



breakpoint_t* getBreakpoints(FILE *fp, long* psize){
	int got; 
	long npoints = 0, size = 64; 
	double lasttime = 0.0; 
	breakpoint_t* points = NULL; 
	char line[80]; // 80 is the line size
	// check if file pointer is empty
	if(fp==NULL){
		return NULL; 
	}

	// allocate breakpoints for size of file
	points = (breakpoint_t*)malloc(sizeof(breakpoint_t) * size)
	if(points == NULL){
		return NULL; 
	}

	while(fgets(line, 80, fp)){
		got = sscanf(
			line, 
			"%lf%lf",
			&points[npoints].time, 
			&points[npoints].value
		); 
		
	}

} 

int main(int argc, char **argv){

	// breakpoint file: Contains time and value
	
	
}
