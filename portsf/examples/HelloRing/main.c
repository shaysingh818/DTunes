#include <stdio.h> 
#include <math.h> 
#include <portaudio.h> 
#include "../include/logging/log.h"


#define FRAME_BLOCK_LEN 256
#define SAMPLING_RATE 44100
#define TWO_PI (3.14159265f * 2.0f)

PaStream *audioStream;
double si = 0; 

int audio_callback(
	const void* inputBuffer, void *outputBuffer, 
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData
) {

	float *in = (float*) inputBuffer, *out = (float*)outputBuffer;
	static double phase = 0;
	unsigned long i;
	for( i=0; i < framesPerBuffer; i++ ) {
		float sine = sin(phase);
		*out++ = *in++ * sine; /* left channel */
		*out++ = *in++ * sine; /* right channel */
		phase += si; 
	}

	return paContinue; 

}

void init(){

	float frequency; 
	int i, id; 
	const PaDeviceInfo *info; 
	const PaHostApiInfo *hostapi; 
	PaStreamParameters outputParameters, inputParameters; 
	dlog("DTUNES", "Type frequency in herts"); 
	scanf("%f", &frequency); 

	si = TWO_PI * frequency / SAMPLING_RATE; 
	dlog("PORTAUDIO", "INIT PORTAUDIO");
	Pa_Initialize(); 

	// get devices
	for (i=0;i < Pa_GetDeviceCount(); i++) {
		info = Pa_GetDeviceInfo(i); /* get information from current device */
		hostapi = Pa_GetHostApiInfo(info->hostApi); /*get info from curr. host API */
		if (info->maxOutputChannels > 0){
			printf("%d: [%s] %s (output)\n",i, hostapi->name, info->name );
		}
	}
 
	dlog("DTUNES", "Type frequency in herts"); 
	scanf("%d", &id);
	info = Pa_GetDeviceInfo(id); 
	hostapi = Pa_GetHostApiInfo(info->hostApi);
	dlog("DTUNES", "Opening audio engine");
	printf("Opening audio device: [%s] %s\n", hostapi->name, info->name); 
	
	// output parameters
	outputParameters.device = id; 
	outputParameters.channelCount = 2;  
	outputParameters.sampleFormat = paFloat32; 
	outputParameters.suggestedLatency = info->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;


	// view devices for input 
	for (i=0;i < Pa_GetDeviceCount(); i++) {
		info = Pa_GetDeviceInfo(i); /* get information from current device */
		hostapi = Pa_GetHostApiInfo(info->hostApi); /*get info from curr. host API */
		if (info->maxInputChannels > 0){
			printf("%d: [%s] %s (input)\n",i, hostapi->name, info->name );
		}
	}

	
	dlog("DTUNES", "Type frequency in herts"); 
	scanf("%d", &id);
	info = Pa_GetDeviceInfo(id); 
	hostapi = Pa_GetHostApiInfo(info->hostApi);
	dlog("DTUNES", "Opening audio engine");
	printf("Opening audio device: [%s] %s\n", hostapi->name, info->name);
 
	// output parameters
	inputParameters.device = id; 
	inputParameters.channelCount = 2;  
	inputParameters.sampleFormat = paFloat32; 
	inputParameters.suggestedLatency = info->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	// open audio stream
	Pa_OpenStream(
		&audioStream, 
		&inputParameters, 
		&outputParameters, 
		SAMPLING_RATE, 
		FRAME_BLOCK_LEN, 
		paClipOff, 
		audio_callback, 
		NULL
	); 

	Pa_StartStream(audioStream); 
	printf("Running...\n"); 
	
}


void terminate(){
	Pa_StopStream( audioStream );
	Pa_CloseStream( audioStream ); 
	Pa_Terminate();
}


void main(int argc, char **argv){

	init();
	while(getchar() != ' ') Pa_Sleep(100); 
	terminate();  
}




