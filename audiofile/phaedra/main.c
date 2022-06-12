#include "phaedra.h"
#include <stdio.h> 
#include <stdlib.h> 

int main(int argc, char **argv){

	printf("testing phaedra library\n");

	// select default device
	PaStreamParameters inputParams; 
	selectDefaultDeviceInfo(inputParams); 
}
