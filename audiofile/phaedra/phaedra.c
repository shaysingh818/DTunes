#include "phaedra.h"


int audio_callback(
    const void* input, void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
){

	float *in = (float*) input, *out = (float*)output;
	static double phase = 0;
	for(int i = 0; i < frameCount; i++){
		float sine = sin(phase);
		*out++ = *in++ * sine; /* left channel */
		*out++ = *in++ * sine; /* right channel */
		phase += SI; 
	}

	return paContinue; 
}


void selectDefaultDeviceInfo(PaStreamParameters params){

	const PaDeviceInfo *info; 
	const PaHostApiInfo *hostapi; 
	// get devices
	for(int i = 0; i < Pa_GetDeviceCount(); i++){
		info = Pa_GetDeviceInfo(i); 
		hostapi = Pa_GetHostApiInfo(info->hostApi); 
		if(info->maxOutputChannels > 0){
			printf("HOSTAPI %s\n", hostapi->name); 		
			printf("HOSTAPI INFO %s\n", info->name); 		
		}
	}
	
}



