#include "phaedra.h"


PaError initPa(void){

	const PaDeviceInfo *inputInfo, *outputInfo; 
	const PaHostApiInfo *inHostapi, *outHostapi; 
	PaStreamParameters inputParams, outputParams, *inPars, *outPars; 
	PaDeviceIndex inDevice, outDevice; 
	PaError err;
	err = Pa_Initialize(); 
	if(err != paNoError) goto error; 

	/* use default device id */ 
	inDevice = Pa_GetDefaultInputDevice();
	
	/* set input stream params */ 
	inputParams.device = inDevice; 
	inputParams.channelCount = 2; 
	inputParams.sampleFormat = SAMPLE_TYPE; 
	inputParams.suggestedLatency =  Pa_GetDeviceInfo(inDevice)->defaultLowInputLatency; 
	inputParams.hostApiSpecificStreamInfo = NULL;
	inPars = &inputParams; 	

	/* debug input device information */ 
	inputInfo = Pa_GetDeviceInfo(inDevice); 
	inHostapi = Pa_GetHostApiInfo(inputInfo->hostApi); 	
	printf("[%s] %s (input)\n", inHostapi->name, inputInfo->name );

	dlog("PHAEDRA", "Using default input device"); 
	printf("Using [%d] INPUT\n", inDevice); 
	
	/* use default device id */ 
	outDevice = Pa_GetDefaultOutputDevice();
	
	/* set input stream params */ 
	outputParams.device = outDevice; 
	outputParams.channelCount = 2; 
	outputParams.sampleFormat = SAMPLE_TYPE; 
	outputParams.suggestedLatency =  Pa_GetDeviceInfo(inDevice)->defaultLowOutputLatency; 
	outputParams.hostApiSpecificStreamInfo = NULL;
	outPars = &outputParams; 

	/* debug output device information */ 
	outputInfo = Pa_GetDeviceInfo(outDevice); 
	outHostapi = Pa_GetHostApiInfo(outputInfo->hostApi); 	
	printf("[%s] %s (output)\n", outHostapi->name, outputInfo->name );
	

	dlog("PHAEDRA", "Using default output device");	
	printf("Using [%d] OUTPUT\n", outDevice); 

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

	/* start the stream */ 
	if(err != paNoError) goto error; 
	err = Pa_StartStream(stream); 
	if(err != paNoError) goto error;  
	

	return err; 
	 	 
	error:
		printf("Error occured while initializing phaedra\n"); 
		printf("Error msg: %s\n", Pa_GetErrorText(err)); 
		Pa_Terminate(); 
		return err; 
	
}

void phaedraInit(){
	PaError err; 
	err = initPa(); 
	if(err){
		printf("an error occurred while initializing tinyAudioLib.\n");
        printf("Error number: %d\n", err );
        printf("Error message: %s\n", Pa_GetErrorText( err ) );
        printf("Exiting.\n");
        exit(-1);

	}

}

void outBlockMono(float *samples, long numsamples){
    static float samp[MAX_FRAMES];
    long j,k;
    initPa();
    for ( j = 0, k=0; j< numsamples; j++, k+=2) {
        samp[k+1] = samp[k] = samples[j];
    }
    Pa_WriteStream( stream, &samp[0], numsamples );
}



void outBlockInterleaved(float *samples, long numframes){
    initPa();
    Pa_WriteStream( stream, samples, numframes );
}

void outBlockStereo(float *samplesLeft, float *samplesRight, long numframes){
    static float samp[MAX_FRAMES];
    long j,k;
    initPa();
    for ( j = 0, k=0; j< numframes; j++, k+=2) {
        samp[k]   = samplesLeft [j];
        samp[k+1] = samplesRight[j];
    }
    Pa_WriteStream( stream, &samp[0], numframes );
}


PANPOS constantpower(double position){
	PANPOS pos; 
	const double piovr2 = 4.0 * atan(1.0) * 0.5; 
	const double root2ovr2 = sqrt(2.0) * 0.5; 
	double thispos = position * piovr2; 
	// each channel uses a 1/4 of a cycle
	double angle = thispos * 0.5; 
	// adjust positions
	pos.left = root2ovr2 * (cos(angle) - sin(angle)); 
	pos.right = root2ovr2 * (cos(angle) + sin(angle)); 
	return pos; 
}


void play(char *filename){
	float buf[MAX_FRAMES]; 
	int sfd; 
	PSF_PROPS props; 
	
	long nread; /* number of frames read */ 
	psf_init(); 
	sfd = psf_sndOpen(filename, &props, 0); 
	if(sfd < 0){ dlog("PHAEDRA", "Error opening file"); }
	if(props.chans > 2) { dlog("PHAEDRA", "Invalid number of channels"); }
	
	/* process frames */

	do {
		nread = psf_sndReadFloatFrames(sfd, buf, 512);
		outBlockMono(buf, 512); 
	
	} while(nread == 512); 

	psf_sndClose(sfd); 
	psf_finish(); 
	
}


