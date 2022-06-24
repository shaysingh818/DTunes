#include "sdl.h"


void pushCommand(char *operation, char *filePath){
	
	/* get current time */ 
	time_t rawtime; 
	struct tm *timeinfo; 
	time(&rawtime); 
	timeinfo = localtime(&rawtime); 
	char *dateTime = asctime(timeinfo); 

	FILE *fp; 
	fp = fopen(SDL_QUEUE_INPUT, "w+"); 
	fprintf(fp, "%s,%s,%s\n", operation, filePath, dateTime); 
	fclose(fp); 
}


void push(audio_t **headRef, char *file, int time, int plays){
	
	/* allocate memory */ 
	audio_t *newFile = (audio_t*)malloc(sizeof(audio_t)); 

	/* insert data */
	newFile->filePath = file; 
	newFile->timeDuration = time; 
	newFile->plays = plays; 

	/* make next of new node as head and prev as NULL */ 
	newFile->nextFile = (*headRef);
	newFile->prevFile = NULL; 

	/* change prev of head to new node */ 
	if((*headRef) != NULL){
		(*headRef)->prevFile = newFile; 
	}

	(*headRef) = newFile; 
	 
}


void insertAfter(audio_t *prevNode, char *file, int time, int plays){

	/* check if prev node is null */
	if(prevNode == NULL){
		printf("Audio node cannot be NULL\n"); 
		return;
	}
	
	/* insert data */
	audio_t *newFile = (audio_t*)malloc(sizeof(audio_t)); 		
	newFile->filePath = file; 
	newFile->timeDuration = time; 
	newFile->plays = plays; 

	/* break the chains */ 
	newFile->nextFile = prevNode->nextFile; 
	prevNode->nextFile = newFile; 
	newFile->prevFile = prevNode; 

	/* change prev node to next */ 
	if(newFile->nextFile != NULL){
		newFile->nextFile->prevFile = newFile; 
	}

}


void append(audio_t **headRef, char *file, int time, int plays){
	
	audio_t *newFile = (audio_t*)malloc(sizeof(audio_t));
	audio_t *lastFile = *headRef;
	
	/* insert data */ 
	newFile->filePath = file; 
	newFile->timeDuration = time; 
	newFile->plays = plays; 
	newFile->nextFile = NULL; 

	if(*headRef == NULL){
		newFile->prevFile = NULL; 
		*headRef = newFile; 
		return; 
	}
	
	while(lastFile->nextFile != NULL){
		lastFile = lastFile->nextFile; 
	}

	lastFile->nextFile = newFile; 
	newFile->prevFile = lastFile; 

	return; 

}

void printAudioNode(audio_t *audioNode){
	printf("File Path: %s\n", audioNode->filePath); 
	printf("Time Duration: %d\n", audioNode->timeDuration); 
	printf("File Plays: %d\n", audioNode->plays); 
}


void printList(audio_t *audioNode){
	
	audio_t *last; 
	/* traversal forward direction */ 
	while(audioNode != NULL){
		printAudioNode(audioNode); 
		last = audioNode; // in case we want to traverse in reverse direction
		audioNode = audioNode->nextFile; 		
	}

	/**printf("\nTraversal in reverse direction \n");
    while (last != NULL) {
        printf(" %d ", last->data);
        last = last->prev;
    }*/
	
}







