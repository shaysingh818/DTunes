#include "phaedra.h"
#include "phaedra_endpoints.h"
#include "phaedra_test.h"
#include "../logging/log.h"


void playAudioCmd(endpoint_t *e, char* argv[]){

	dlog("PHAEDRA", "Play audio file"); 
	play(argv[3], 2);

}


void playAudioCallbackCmd(endpoint_t *e, char* argv[]){

	dlog("PHAEDRA", "Play audio file with callback"); 
	//playCallback(argv[2]); 


}

void writeToQueue(endpoint_t *e, char* argv[]){
	
	queue_t *q = initQueue(1000); 
	enqueue(q, "phaedra/test_files/bruises.wav"); 
	enqueue(q, "phaedra/test_files/hill.wav"); 
	enqueue(q, "phaedra/test_files/disclosure.wav"); 

	dequeue(q); 
	dequeue(q);
	playQueue(q);  	
}


void testPhaedraCmd(endpoint_t *e, char* argv[]){

	// run test cases for phaedra
	runTests(); 
}


page_t *phaedraModule(){

	// create endpoint  
    endpoint_t *head = NULL;

	// play audio file
    char *playArgs[][2] = {
        {"play", "play action"},
        {"name", "Name of the file you want to play"},
    };

    constructEndpoint(
        "play",
        "play wav, aiff or mp3 file",
        0, 2,
        playArgs,
        playAudioCmd,
        &head
    );

	// play audio file with callback
    char *callbackArgs[][2] = {
        {"play-callback", "play with audio callback"},
        {"name", "Name of the file you want to play"},
    };

    constructEndpoint(
        "play-callback",
        "play wav, aiff or mp3 file with callback",
        0, 2,
        callbackArgs,
        playAudioCallbackCmd,
        &head
    );

	// play audio file queue
    char *queueArgs[][2] = {
        {"queue", "play audio files with queue"},
        {"name", "Name of the file you want to play"},
    };

    constructEndpoint(
        "queue",
        "play audio files with queue",
        0, 2,
        queueArgs,
        writeToQueue,
        &head
    );

	// test phaedra
    char *testArgs[][2] = {
        {"test", "run tests for phaedra"}
    };

    constructEndpoint(
        "test",
        "test functionality for phaedra",
        0, 1,
        testArgs,
        testPhaedraCmd,
        &head
    );

	// create page
    page_t *p = createPage(
        "phaedra",  
        "Audio player module for DTunes",
        head
    );

    return p; 
	
}


