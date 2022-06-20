#include "phaedra.h"

int audioCallback(const void *input, void *output,
				unsigned long frameCount,
				const PaStreamCallbackTimeInfo *timeInfo,
				PaStreamCallbackFlags statusFlags,
				void *userData)
{
	/* grab callback data */ 
	float *out; 
	mydata *p = (mydata*)userData; 
	int sf = p->sf;
	long nread = p->nread; 

	out = (float*)output; 
	memset(out, 0, sizeof(float) * MAX_FRAMES);

	nread = psf_sndReadFloatFrames(sf, out, 512);
	printf("NREAD: %ld\n", nread); 

	if(nread < frameCount){
		return paComplete; 
	}

	return paContinue;
}


PaStreamParameters* setInputParams(PaStreamParameters *inputParams){
	
	/* in device */ 
	PaDeviceIndex inDevice = Pa_GetDefaultInputDevice();
	PaStreamParameters *inPars; 
	
	/* set latency */ 
	if(inDevice >= 0 && inLatency < 0){
		inLatency = Pa_GetDeviceInfo(inDevice)->defaultLowInputLatency;
		if(inLatency <= 0) {
			inLatency = 0.2; 
		}
	}

	/* set input stream params */
	inputParams->device = inDevice; 
	inputParams->channelCount = 2; 
	inputParams->sampleFormat = SAMPLE_TYPE; 
	inputParams->suggestedLatency =  inLatency; 
	inputParams->hostApiSpecificStreamInfo = NULL;
	inPars = inputParams; 
	return inPars;  

}


PaStreamParameters* setOutputParams(PaStreamParameters *outputParams){

	/* out device */ 
	PaDeviceIndex outDevice = Pa_GetDefaultOutputDevice(); 
	PaDeviceIndex inDevice = Pa_GetDefaultInputDevice();
	PaStreamParameters *outPars;

	
	/* set latency */ 
	if(outDevice >= 0 && outLatency < 0){
		outLatency = Pa_GetDeviceInfo(outDevice)->defaultLowOutputLatency;
		if(inLatency <= 0) {
			inLatency = 0.2; 
		}
	}
 	
	/* set output stream parameters */ 
	outputParams->device = outDevice; 
	outputParams->channelCount = 2; 
	outputParams->sampleFormat = SAMPLE_TYPE; 
	outputParams->suggestedLatency =  outLatency; 
	outputParams->hostApiSpecificStreamInfo = NULL;
	outPars = outputParams; 
	return outPars; 
}

/* create bool that does it with a callback or not */ 
PaError initStream(mydata *data, int callback){

	PaStreamParameters inputParams, outputParams, *inPars, *outPars; 
	PaError err;
	err = Pa_Initialize(); 
	if(err != paNoError) goto error; 
	
	/* set input params */ 	
	inPars = setInputParams(&inputParams); 

	/* set output params */ 
	outPars = setOutputParams(&outputParams); 

	if(callback){

		printf("Using audio callback stream\n"); 
		/* create stream with callback */
		err = Pa_OpenStream(
			&stream, 
			inPars, 
			outPars, 
			SAMPLING_RATE, 
			FRAME_BLOCK_LEN, 
			paNoFlag, 
			audioCallback, 
			data
		);
	}else {
		
		printf("Using blocking API\n"); 
		err = Pa_OpenStream(
			&stream, 
			inPars, 
			outPars, 
			SAMPLING_RATE, 
			FRAME_BLOCK_LEN, 
			paNoFlag, 
			NULL, 
			NULL
		);

	}

	/* start the stream */ 
	if(err != paNoError) goto error; 

	return err; 
	 	 
	error:
		printf("Error occured while initializing phaedra\n"); 
		printf("Error msg: %s\n", Pa_GetErrorText(err)); 
		Pa_Terminate(); 
		return err; 
	
}


/* frame reading functions for later use */ 
void outBlockMono(float *samples, long numsamples){
	static float samp[MAX_FRAMES];
	long j,k;
	//initPa(); 
	for ( j = 0, k=0; j< numsamples; j++, k+=2) {
		samp[k+1] = samp[k] = samples[j];
	}

	Pa_WriteStream(stream, &samp[0], numsamples );
}

void outBlockInterleaved(float *samples, long numframes){
	//initPa(); 
	Pa_WriteStream( stream, samples, numframes );
}


void inSampleStereo(float *sampleLeft, float *sampleRight){
	float samples[2];
	//if (!isOpened) initPablio();
	Pa_ReadStream( stream, samples, 1 );
	*sampleLeft = samples[0];
	*sampleRight = samples[1];
}


void outSampleStereo(float sampleLeft, float sampleRight){
	float samples[2];
	//if (!isOpened) initPablio();
	samples[0] = sampleLeft;
	samples[1] = sampleRight;
	Pa_WriteStream( stream, &samples[0], 1 );
}



float inSampleMono(void) {
	float samples[2];
	//if (!isOpened) initPablio();
	Pa_ReadStream( stream, samples, 1 );
	return samples[0] + samples[1]; /* mix left and right channels */
}


void outSampleMono(float sample) {
	int err;
	float samples[2];
	//if (!isOpened) initPablio();
	samples[0] = sample;
	samples[1] = sample;
	err = Pa_WriteStream( stream, &samples[0], 1 );
}



char* checkSampleType(psf_stype type){
	char *bitRate; 
    switch(type){
        case(PSF_SAMP_8):
			bitRate = "8 bit sample rate"; 
            break;
        case(PSF_SAMP_16):
            bitRate = "16 bit sample";
            break;
        case(PSF_SAMP_24):
            bitRate = "24 bit sample rate";
            break;
        case(PSF_SAMP_32):
            bitRate = "32 bit sample rate";
            break;
        case(PSF_SAMP_IEEE_FLOAT):
            bitRate = "32 bit floating point";
            break;
        default:
            bitRate = "Unknown";
    }

    return bitRate;
}



void displayAudioInformation(char *filename){

	float buf[MAX_FRAMES]; 
	int sfd; 
	PSF_PROPS props; 
	sfd = psf_sndOpen(filename, &props, 0); 
	if(sfd < 0){ dlog("PHAEDRA", "Error opening file"); }	

	char *result = checkSampleType(props.samptype); 
	dlog("SAMPLE TYPE", result); 
	dlog_int("CHANNELS", props.chans); 
}


void playCallback(char *filename){

	int sfd;
    long nread;
    float buf[MAX_FRAMES];
	PaError err; 
    PSF_PROPS props;
    psf_init();
    sfd = psf_sndOpen(filename, &props, 0);
    if(sfd < 0){ dlog("PHAEDRA", "Error opening file"); }
    if(props.chans > 2) { dlog("PHAEDRA", "Invalid number of channels"); }

	mydata *data = (mydata*)malloc(sizeof(mydata)); 
	data->sf = sfd; 
	err = initStream(data, 1); 
	
	/* play stream */
	if(err == paNoError){
		err = Pa_StartStream(stream);
		if(err == paNoError){
			while(Pa_GetStreamTime(stream) < 60); 
			
		}
	}
}

void play(char *filename){

	/* portsf */ 	 
    int sfd;
	long nread; 
	float buf[MAX_FRAMES]; 
	PSF_PROPS props;
    psf_init();
    sfd = psf_sndOpen(filename, &props, 0);
    if(sfd < 0){ dlog("PHAEDRA", "Error opening file"); }
    if(props.chans > 2) { dlog("PHAEDRA", "Invalid number of channels"); }

	/* start stream */ 	
	PaError err;
	initStream(NULL, 0); 
	err = Pa_StartStream(stream); 
	if(err != paNoError) goto error;

	/* non blocking streaming method */ 	
	while(Pa_GetStreamTime(stream) < 60){
		err = Pa_ReadStream(stream, buf, 512); 
		if(err == paNoError){
			do {
				nread = psf_sndReadFloatFrames(sfd, buf, 512);
				Pa_WriteStream( stream, buf, 512);

			} while(nread == 512); 
		}else{
			printf("%s \n", Pa_GetErrorText(err)); 
		}
		Pa_CloseStream(stream); 
		Pa_Terminate(); 
	} 

		
	error:
		printf("Error occured when starting stream\n"); 
		printf("Error msg: %s\n", Pa_GetErrorText(err)); 
		Pa_Terminate(); 


	psf_sndClose(sfd); 
	psf_finish();	

	
}


