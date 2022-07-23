#include "phaedra.h"
#include "../logging/log.h"


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

	nread = psf_sndReadFloatFrames(sf, out, FRAME_BLOCK_LEN);

	if(nread < frameCount){
		printf("Song is done\n"); 
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


PaError initStream(void){

	PaStreamParameters inputParams, outputParams, *inPars, *outPars; 
	PaError err;
	err = Pa_Initialize(); 
	if(err != paNoError) goto error; 
	
	/* set input and output params */ 	
	inPars = setInputParams(&inputParams); 
	outPars = setOutputParams(&outputParams); 
		
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


PaError initCallbackStream(mydata *data){

	PaStreamParameters inputParams, outputParams, *inPars, *outPars; 
	PaError err;
	err = Pa_Initialize(); 
	if(err != paNoError) goto error; 
	
	/* set input and output params */ 	
	inPars = setInputParams(&inputParams); 
	outPars = setOutputParams(&outputParams);

	/* create audio callback stream */ 
	printf("Created callback stream\n"); 
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


PaError closeStream(void){

	PaError err; 
	err = Pa_CloseStream(stream); 
	if (err != paNoError) goto error; 
	err = Pa_Terminate(); 	
	if (err != paNoError) goto error; 
	dlog("PHAEDRA", "Closing audio stream"); 
	return err; 

	error:
		dlog("ERROR", "Closing stream"); 
		printf("Error msg: %s\n", Pa_GetErrorText(err)); 
		Pa_Terminate(); 
		return err; 
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



void play(char *filename, int streamType){

	/**
		This is the first prototyped method of a function that plays a wav file
		The real time solutions for portaudio are not perfected, for now,  i'm going
		to use the outBlockInterleaved stream writing function
	*/ 

	int sfd;
    long nread;
    float buf[MAX_FRAMES];
	PaError err; 
    PSF_PROPS props;
    psf_init();
    sfd = psf_sndOpen(filename, &props, 0);
    if(sfd < 0){ dlog("PHAEDRA", "Error opening file"); } // exit program here }
    if(props.chans > 2) { dlog("PHAEDRA", "Invalid number of channels"); }
	
	/* check stream type and initialize */
	if(streamType == 1){

		err = initStream(); 
		if(err) {
			dlog("STREAM ERROR", "Intializing stream"); 
			dlog_int("ERROR NUMBER", err); 
			printf("Error msg: %s\n", Pa_GetErrorText(err)); 
			exit(-1); 
		}

		do {
			nread = psf_sndReadFloatFrames(sfd, buf, FRAME_BLOCK_LEN);  
			Pa_WriteStream(stream, buf, FRAME_BLOCK_LEN);
		} while(nread == FRAME_BLOCK_LEN); 

	}else if(streamType == 2){

		/* create callback data to pass to stream */ 
		mydata *data = (mydata*)malloc(sizeof(mydata)); 
		data->sf = sfd; 

		err = initCallbackStream(data);
		if(err) {
			dlog("STREAM ERROR", "Intializing callback stream"); 
			dlog_int("ERROR NUMBER", err); 
			printf("Error msg: %s\n", Pa_GetErrorText(err)); 
			exit(-1); 
		}

		/* play stream here */
		while(Pa_IsStreamActive(stream)){
			Pa_Sleep(100); 
		}

		free(data); 
		
	}else {

		dlog("PHAEDRA", "No stream type selected"); 
		exit(-1); 
	}

 

	psf_sndClose(sfd); 
	psf_finish();

	err = closeStream(); 
	dlog("PHAEDRA", "Closed audio file stream");  


}



queue_t* initQueue(){
	queue_t* q = (queue_t*)malloc(sizeof(queue_t)); 
	q->front = q->rear = NULL; 
	return q; 
}


audionode_t *createNode(char *filePath){
	audionode_t *temp = (audionode_t*)malloc(sizeof(audionode_t)); 
	temp->filePath = filePath; 
	/* get current time of insert */
	time_t rawtime; 
	struct tm *timeinfo; 
	time(&rawtime); 
	timeinfo = localtime(&rawtime);  
	temp->currentTime = asctime(timeinfo); 
	temp->plays = 0;
	return temp;  
}


void pushToQueue(queue_t *q, char *filePath){

	audionode_t *temp = createNode(filePath); 
	if(q->rear == NULL){
		q->front = q->rear = temp; 
		return; 
	}
	q->rear->next = temp; 
	q->rear = temp;
	dlog("AUDIO QUEUE", "PUSHED AUDIO FILE ONTO QUEUE"); 
}


void removeFromQueue(queue_t *q){

	if(q->front == NULL){
		return; 
	}
	audionode_t *temp = q->front; 
	q->front = q->front->next; 
	
	if(q->front == NULL){
		q->rear = NULL; 
	}

	free(temp); 
	dlog("AUDIO QUEUE", "REMOVED AUDIO FILE FROM QUEUE"); 
}



void printQueue(queue_t *q){
	while(q->front != NULL){
		printf("File on queue: %s\n", q->front->filePath); 
		removeFromQueue(q); 
	}
}


void playQueue(queue_t *q){
	while(q->front != NULL){	
		play(q->front->filePath, 2); 
		removeFromQueue(q); 
	}
}
