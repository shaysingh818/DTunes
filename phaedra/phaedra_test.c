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

	/**
    PaError err;

	err = initStream();
    if(err) {	
		printf("\e[0;31m");
		dlog("FAILED", "COULD NOT PHAEDRA STREAM");
        printf("Error msg: %s\n", Pa_GetErrorText(err));
        exit(-1);
   }

	printf("\033[0;32m");	
	dlog("PASSED", "CREATED PHAEDRA STREAM"); */

}


void testCallBackStream(){

	/**
    PaError err;

	err = initStream();
    if(err) {	
		printf("\e[0;31m");
		dlog("FAILED", "COULD NOT PHAEDRA STREAM");
        printf("Error msg: %s\n", Pa_GetErrorText(err));
        exit(-1);
   }

	printf("\033[0;32m");	
	dlog("PASSED", "CREATED PHAEDRA STREAM"); */
}


void testLoadQueue(){

	// test loading items into a queue
	queue_t *q = initQueue(1000);

	// load 5 random items
	enqueue(q, "test1");
	enqueue(q, "test2");
	enqueue(q, "test3");
	enqueue(q, "test4");
	enqueue(q, "test5");

	// declare comparison array
	char *compareNames[10] = {
		"test1", "test2", "test3","test4"
	};

	// check if file names are equal
	for(int i = q->frontIndex; i < q->rearIndex; i++){
		char *filename = q->items[i]->filePath; 
		if(strcmp(filename, compareNames[i]) != 0){
			printf("\e[0;31m");
			dlog("FAILED", "LOAD PHAEDRA QUEUE"); 
			break; 
		}
	}


	printf("\033[0;32m");	
	dlog("PASSED", "LOAD PHAEDRA QUEUE"); 	

}


void testPopQueue(){



	// test loading items into a queue
	queue_t *q = initQueue(1000);

	// load 5 random items
	enqueue(q, "test1");
	enqueue(q, "test2");
	enqueue(q, "test3");
	enqueue(q, "test4");
	enqueue(q, "test5");

	// declare comparison array
	char *compareNames[10] = {
		"test1", "test2", "test3"
	};

	// pop items
	dequeue(q); 

	// check if file names are equal
	for(int i = 0; i < 3; i++){ 
		char *filename = q->items[i]->filePath; 
		if(strcmp(filename, compareNames[i]) != 0){
			printf("\e[0;31m");
			dlog("FAILED", "POP PHAEDRA QUEUE"); 
			break; 
		}
	}
	
	printf("\033[0;32m");	
	dlog("PASSED", "POP PHAEDRA QUEUE"); 	
}


void testInitQueue() {

	// test loading items into a queue
	queue_t *q = initQueue(1000);

	// add items 	
	enqueue(q, "test1");
	enqueue(q, "test2");
	enqueue(q, "test3");
	enqueue(q, "test4");
	enqueue(q, "test5");

	// test if item count is correct
	if(q->itemCount == 5){		
		printf("\e[0;32m");
		dlog("PASSED", "QUEUE COUNT"); 
	}else {
		printf("\033[0;31m");	
		dlog("FAILED", "QUEUE COUNT"); 
	}


	// test is empty method
	for(int i = 0; i < 5; i++){
		dequeue(q); 
	}

	if(isEmpty(q)){	
		printf("\e[0;32m");
		dlog("PASSED", "QUEUE EMPTY"); 
	}else {

		printf("\e[0;31m");
		dlog("FAILED", "QUEUE EMPTY"); 
	}

	// make queue full
	for(int i = 0; i < 1000; i++){
		enqueue(q, "a"); 
	}

	// check if queue is full
	if(isFull(q)){	
		printf("\e[0;32m");
		dlog("PASSED", "QUEUE FULL"); 
	}else {

		printf("\e[0;31m");
		dlog("FAILED", "QUEUE FULL"); 
	}

}


void runTests() {

	testSetIOParams();
	testInitStream();  
	testCallBackStream(); 
	testLoadQueue(); 
	testPopQueue();
	testInitQueue();  
	

}
