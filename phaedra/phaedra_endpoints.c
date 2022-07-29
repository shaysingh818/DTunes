#include "phaedra.h"
#include "phaedra_endpoints.h"
#include "phaedra_test.h"
#include "../logging/log.h"


void playAudioCmd(endpoint_t *e, char* argv[]){

	if (strcmp(argv[1], e->commandLineArg) == 0){
		dlog("PHAEDRA", "Play audio file"); 
		play(argv[2], 2); 
	}	

}


void playAudioCallbackCmd(endpoint_t *e, char* argv[]){

	if (strcmp(argv[1], e->commandLineArg) == 0){
		dlog("PHAEDRA", "Play audio file with callback"); 
		//playCallback(argv[2]); 
	}	

}


void writeToQueue(endpoint_t *e, char* argv[]){
	
	if (strcmp(argv[1], e->commandLineArg) == 0){

		queue_t *q = initQueue(1000); 
		enqueue(q, "phaedra/test_files/bruises.wav"); 
		enqueue(q, "phaedra/test_files/hill.wav"); 
		enqueue(q, "phaedra/test_files/disclosure.wav"); 

		dequeue(q); 
		dequeue(q);
		playQueue(q);  

		/**
		queue_t *q = initQueue();
	
		pushToQueue(q, "phaedra/test_files/bruises.wav"); 
		pushToQueue(q, "phaedra/test_files/hill.wav"); 
		pushToQueue(q, "phaedra/test_files/disclosure.wav"); 
		pushToQueue(q, "phaedra/test_files/amiwrong.wav");
		playQueue(q); 
		*/

	}	
}



void phaedraEndpoints(endpoint_t **head){

	// play audio endpoint 
	endpoint_t *e1 = createEndpoint(
		"play-audio",
		"pa",
		"play wav, aiff or mp3 file"
	);	
	e1->endpointLogic = playAudioCmd; 
	appendEndpoint(head, e1);
 
	// play audio endpoint 
	endpoint_t *e2 = createEndpoint(
		"play-audio-callback",
		"pac",
		"play wav, aiff or mp3 file with callback"
	);	
	e2->endpointLogic = playAudioCallbackCmd; 
	appendEndpoint(head, e2); 
	 
	// show audio file info	
	endpoint_t *e3 = createEndpoint(
		"queue",
		"qu",
		"add audio files to queue"
	);	
	e3->endpointLogic = writeToQueue; 
	appendEndpoint(head, e3);

 
}
