#include <stdio.h> 
#include <math.h> 
#include <portaudio.h> 
#include "logging/log.h"

#define FRAME_BLOCK_LEN 512
#define SAMPLING_RATE 44100
#define TWO_PI (3.14159265f * 2.0f)
#define NUM_CHANNELS (1)
#define SI (0)

struct Player {
	char name[20];  
	PaStream *audioStream; 
};

typedef struct Player player_t; 

int audio_callback(
	const void* input, void *output, 
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData
);
 
void selectDefaultDeviceInfo(PaStreamParameters params); 



