#include "phaedra_test.h"
#include "phaedra.h"
#include "../logging/log.h"

void testSetIOParams(){

	/* create input params */ 
	PaStreamParameters inputParams, outputParams, *inPars, *outPars;
	PaError err;
	err = Pa_Initialize();
 
	inPars = setInputParams(&inputParams); 
	outPars = setInputParams(&inputParams); 

	if(!inPars || !outPars){
		printf("\e[0;31m");
		dlog("FAILED", "COULD NOT SET IO PARAMS");
	}

	printf("\033[0;32m");	
	dlog("PASSED", "SET IO PARAMS");

}


void testInitStream(){

    PaError err;

	err = initStream();
    if(err) {	
		printf("\e[0;31m");
		dlog("FAILED", "COULD NOT PHAEDRA STREAM");
        printf("Error msg: %s\n", Pa_GetErrorText(err));
        exit(-1);
   }

	printf("\033[0;32m");	
	dlog("PASSED", "CREATED PHAEDRA STREAM");

}


void testCallBackStream(){

    PaError err;

	err = initStream();
    if(err) {	
		printf("\e[0;31m");
		dlog("FAILED", "COULD NOT PHAEDRA STREAM");
        printf("Error msg: %s\n", Pa_GetErrorText(err));
        exit(-1);
   }

	printf("\033[0;32m");	
	dlog("PASSED", "CREATED PHAEDRA STREAM");

}

void runTests() {

	testSetIOParams();
	testInitStream();  

}
