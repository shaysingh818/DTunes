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

struct Player {
	char name[20];  
	PaStream *audioStream; 
};

typedef struct panpos {
    double left;
    double right;
} PANPOS;

static int init = 0;

PaStream *stream; /* default stream for now */  

typedef struct Player player_t; 

/* init functions */  
PaError initPa(void); 
void phaedraInit(); 

/* frame reading */ 
void outBlockMono(float *samples, long numsamples); 
void outBlockInterleaved(float *samples, long numframes); 

/* play wav file */ 
void play(char *filename); 




