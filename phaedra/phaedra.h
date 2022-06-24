#include <stdio.h> 
#include <math.h> 
#include <portaudio.h>
#include <portsf.h>  
#include "logging/log.h"

#define FRAME_BLOCK_LEN 512

#define SAMPLING_RATE 44100
#define SAMPLE_TYPE  paFloat32
#define NUM_CHANNELS (1)
#define MAX_FRAMES (512 * 2)

#define TWO_PI (3.14159265f * 2.0f)
#define SI (0)

static double inLatency = -1;
static double outLatency = -1;

typedef struct CallbackData {
	int sf;
	long nread; 
} mydata; 


PaStream *stream; /* default stream for now */  

/* callback function */ 
int audioCallback(
	const void *input, void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo *timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData
);


/* original init function */
PaStreamParameters* setInputParams(PaStreamParameters *inputParams);
PaStreamParameters* setOutputParams(PaStreamParameters *outputParams); 
PaError initStream(mydata *data, int callback); 

/* frame reading functions */ 
void outBlockMono(float *samples, long numsamples); 
void outBlockInterleaved(float *samples, long numframes); 
void inSampleStereo(float *sampleLeft, float *sampleRight); 
void outSampleStereo(float sampleLeft, float sampleRight); 
float inSampleMono(void); 
void outSampleMono(float sample); 

/* check sample type */ 
char *checkSampleType(psf_stype type);
 
/* play wav file */
void displayAudioInformation(char *filename); 
void playCallback(char *filename);  
void play(char *filename);

/* testing the sdl queue */ 
void sdlQueue();  

