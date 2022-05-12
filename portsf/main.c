/**
Write sinewave as text
*/


#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "include/portsf.h"


#ifndef M_PI
#define M_PI (3.141592654)
#endif

void scaleProgram(){
	int note, i; 
	char key[3]; 
	
}


void generateSineWave(){	
	int i,nsamps;
	double samp;
	double twopi = 2.0 * M_PI;
	double angleincr;
	/* set number of points to create */	
	nsamps = 50;
	/* make one complete cycle */
	angleincr = twopi / nsamps;
	for(i=0; i < nsamps; i++){
		samp = sin(angleincr *i);
		fprintf(stdout,"%.8lf\n",samp);
	}
	fprintf(stderr,"done\n");
}

int main(int argc, char** argv){
	
	generateSineWave(); 
	return 0;

}
