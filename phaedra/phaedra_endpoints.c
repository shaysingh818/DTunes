#include "phaedra.h"
#include "phaedra_endpoints.h"


void playAudioCmd(endpoint_t *e, char* argv[]){

	if (strcmp(argv[1], e->commandLineArg) == 0){
		dlog("PHAEDRA", "Play audio file"); 
		play(argv[2]); 
	}	

}


void playAudioCallbackCmd(endpoint_t *e, char* argv[]){

	if (strcmp(argv[1], e->commandLineArg) == 0){
		dlog("PHAEDRA", "Play audio file with callback"); 
		playCallback(argv[2]); 
	}	

}


void checkSampleTypeCmd(endpoint_t *e, char* argv[]){

	if (strcmp(argv[1], e->commandLineArg) == 0){
		displayAudioInformation(argv[2]); 
	}	
}


void testSDLQueue(endpoint_t *e, char* argv[]){
	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		sdlQueue();  
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
		"check-sampletype",
		"cs",
		"check audio sample type"
	);	
	e3->endpointLogic = checkSampleTypeCmd; 
	appendEndpoint(head, e3); 


	// testing SDL queue	
	endpoint_t *e4 = createEndpoint(
		"test-sdl-queue",
		"sdl",
		"add files to the sdl queue"
	);	
	e4->endpointLogic = testSDLQueue; 
	appendEndpoint(head, e4); 

}
