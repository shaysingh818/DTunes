#ifndef SDL_H
#define SDL_H

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#define SDL_QUEUE_INPUT "../logs/sdl.csv"

struct AudioNode {
	char *filePath;
	int timeDuration;
	int plays; 
	struct AudioNode* nextFile; 
	struct AudioNode* prevFile; 
}; 

typedef struct AudioNode audio_t; 

/* input file methods */
void pushCommand(char* operation, char *filePath); 


/* in memory methods */ 
void push(audio_t **headRef, char *file, int time, int plays);
void insertAfter(audio_t *prevNode, char *file, int time, int plays);
void append(audio_t **headRef, char *file, int time, int plays); 
void printAudioNode(audio_t *audioNode);  
void printList(audio_t *audioNode); 

#endif
