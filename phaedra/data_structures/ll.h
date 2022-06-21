#include <stdio.h> 
#include <stdlib.h> 


struct AudioNode {
	char *filePath;
	int timeDuration;
	int plays; 
	struct AudioNode* nextFile; 
	struct AudioNode* prevFile; 
}; 

typedef struct AudioNode audio_t; 

void push(audio_t **headRef, char *file, int time, int plays);
void insertAfter(audio_t *prevNode, char *file, int time, int plays);
void append(audio_t **headRef, char *file, int time, int plays); 
void printAudioNode(audio_t *audioNode);  
void printList(audio_t *audioNode);  
