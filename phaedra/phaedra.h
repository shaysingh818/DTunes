#ifndef PHAEDRA_H
#define PHAEDRA_H

#include <stdio.h> 
#include <string.h> 
#include <stdbool.h> 
#include <math.h> 
#include <portaudio.h>
#include <portsf.h>  

#define FRAME_BLOCK_LEN 512

#define SAMPLING_RATE 44100
#define SAMPLE_TYPE  paFloat32
#define NUM_CHANNELS (1)
#define MAX_FRAMES (512 * 2)
#define END 60

#define TWO_PI (3.14159265f * 2.0f)
#define SI (0)


static double inLatency = -1;
static double outLatency = -1;

PaStream *stream; /* default stream for now */  

typedef struct CallbackData {
	int sf;
	long nread; 
} mydata; 


/* audio node stored at each slot on the queue */ 
struct AudioNode {
	char *filePath; 
	char *currentTime; 
	int plays; 
	struct AudioNode* next; 
};

typedef struct AudioNode audionode_t; 


/* reference for audio queue */ 
struct AudioQueue {
	audionode_t *front, *rear; 
}; 

typedef struct AudioQueue queue_t; 


/* audio callback function */ 
int audioCallback(
	const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo *timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData
);

/* utlities for creating audio streams */
PaStreamParameters* setInputParams(PaStreamParameters *inputParams);
PaStreamParameters* setOutputParams(PaStreamParameters *outputParams); 
PaError initStream(void);
PaError initCallbackStream(mydata *data); 
PaError closeStream(void);  


/* check sample type */ 
char *checkSampleType(psf_stype type);
 
/* play wav file */
void play(char *filename, int streamType);


/* queueing functions */ 
queue_t* initQueue(); 
audionode_t* createNode(char *filePath); 
void pushToQueue(queue_t *q, char *filePath); 
void removeFromQueue(queue_t *q); 
void printQueue(queue_t *q); 
void playQueue(queue_t *q);  


#endif