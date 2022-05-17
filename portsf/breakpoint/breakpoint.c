#include "breakpoint.h"


float larger(float first, float second){
	if(first < second){
		return second; 
	}else{
		return first; 
	}
}


breakpoint_t maxpoint(const breakpoint_t* points, long npoints){
	// create a temp breakpoint
	breakpoint_t point; 
	point.time = points[0].time; 
	point.value = points[0].value; 
	// check for largest point
	for(int i = 0; i < npoints; i++){
		if(point.value < points[i].value){
			point.value = points[i].value; 
			point.time = points[i].time; 
		}
	}

	return point; 
} 


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
	points = (breakpoint_t*)malloc(sizeof(breakpoint_t) * size);
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

		if(got < 0){
			continue; 
		}

		if(got == 0){
			printf("Line %ld has non numeric data\n", npoints+1); 
			break; 
		}

		if(got == 1){
			printf("Incomplete breakpoint found at point %ld\n", npoints+1); 
			break; 
		}

		if(points[npoints].time < lasttime){
			printf("data error at point %ld: time not increasing\n", npoints+1);
			break; 
		}

		lasttime = points[npoints].time; 
		if(++npoints == size){
			breakpoint_t* tmp; 
			size += NPOINTS; 
			tmp = (breakpoint_t*)realloc(points, sizeof(breakpoint_t) * size); 
			if(tmp == NULL){
				npoints = 0; 
				free(points); 
				points = NULL; 
				break; 
			}
			points = tmp; 
		}	
	}


	if(npoints){
		*psize = npoints; 
	}

	return points; 

} 

int main(int argc, char **argv){

	// breakpoint file: Contains time and value
	long size; 
	double dur; 
	breakpoint_t point, *points; 
	FILE* fp; 

	if(argc < 2){
		printf("Usage: breakpoint infile.txt \n"); 
		return 0; 
	}

	fp = fopen(argv[1], "r"); 
	if(fp == NULL){
		return 0; 
	}

	size = 0; 
	points = getBreakpoints(fp, &size);
	if(points == NULL){
		printf("no breakpoints in file\n"); 
		fclose(fp); 
		return 1; 
	} 

	if(size < 2){
		printf("Need at least two breakpoints\n"); 
		free(points); 
		fclose(fp); 
		return 1; 
	}

	if(points[0].time != 0){
		printf("Error in breakpoint file, first time must be 0.0\n"); 
		free(points); 
		fclose(fp); 
		return 1; 
	}

	printf("read %ld breakpoint\n", size); 
	dur = points[size-1].time; 
	printf("Duration: %f seconds\n", dur); 
	point = maxpoint(points, size); 
	printf("Maximum value %f at %f secs \n", point.value, point.time); 
	free(points); 
	fclose(fp); 
	return 0; 

	
}
