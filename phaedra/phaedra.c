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


PaError closeStream(player_t *player){

	player->err = Pa_CloseStream(player->stream); 
	if (player->err != paNoError) goto error; 
	player->err = Pa_Terminate(); 	
	if (player->err != paNoError) goto error; 
	dlog("PHAEDRA", "Closing audio stream"); 
	return player->err; 

	error:
		dlog("ERROR", "Closing stream"); 
		printf("Error msg: %s\n", Pa_GetErrorText(player->err)); 
		Pa_Terminate(); 
		return player->err; 
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


void *playerController(void *playerArgs){
	
	thread_args_t *args = (thread_args_t*)playerArgs; 
	player_t *player = args->player; 
	char *filePath = args->filePath; 

	while(true){
		char key = getchar(); 
    	switch(key){
        	case 'D':
            	printf("Go forward in queue\n");
            	break;
        	case 'A':
            	printf("Go back\n");
            	break;
        	case 'S':
				player->playing = false; 
				pauseFile(player, filePath); 
            	break;
        	case 'P':
            	printf("Player");
            	break;
    	}

	}
}


PaError initPlayerStream(player_t *player){

	PaStreamParameters inputParams, outputParams, *inPars, *outPars; 
	player->err = Pa_Initialize(); 
	if(player->err != paNoError) goto error;

	inPars = setInputParams(&inputParams); 
	outPars = setOutputParams(&outputParams); 

	player->err = Pa_OpenStream(
		&player->stream, 
		inPars, 
		outPars, 
		SAMPLING_RATE, 
		FRAME_BLOCK_LEN, 
		paNoFlag, 
		NULL, 
		NULL
	);

	/* start the stream */ 
	if(player->err != paNoError) goto error;
	player->err = Pa_StartStream(player->stream);  
	if(player->err != paNoError) goto error;

	return player->err; 

	error:
	 	printf("Error occured while initializing phaedra\n"); 
		printf("Error msg: %s\n", Pa_GetErrorText(player->err)); 
		Pa_Terminate(); 
		return player->err;	
}



player_t *initPlayer(bool callback){


	// create instance of player struct
	player_t *player;
	player = (player_t*)malloc(sizeof(player_t)); 
	player->duration = 0; 
	player->playing = false; 
	
	if(callback == false){
	
		player->err = initPlayerStream(player); 
		if(player->err) {
			dlog("STREAM ERROR", "Intializing player stream"); 
			dlog_int("ERROR NUMBER", player->err); 
			printf("Error msg: %s\n", Pa_GetErrorText(player->err)); 
			exit(-1); 
		}

	} else {
	
		/* create callback data to pass to stream */ 
		mydata *data = (mydata*)malloc(sizeof(mydata)); 
		data->sf = player->sfd; 

		player->err = initCallbackStream(data);
		if(player->err) {
			dlog("STREAM ERROR", "Intializing callback stream"); 
			dlog_int("ERROR NUMBER", player->err); 
			printf("Error msg: %s\n", Pa_GetErrorText(player->err)); 
			exit(-1); 
		}

	}

	return player; 

}

void *play(void *playerArgs){

	thread_args_t *args = (thread_args_t*)playerArgs; 
	player_t *player = args->player; 
	char *filePath = args->filePath;

	dlog("FILE", filePath);  

    psf_init();
	player->filePath = filePath; 
    player->sfd = psf_sndOpen(filePath, &player->props, 0);
    if(player->sfd < 0){ dlog("PHAEDRA", "Error opening file"); } 
    if(player->props.chans > 2) { dlog("PHAEDRA", "Invalid number of channels"); }

	int counter = 0; 	
	if(player->duration != 0){
		counter = (long)(player->duration * player->props.srate + counter);
	}

	psf_sndSeek(player->sfd, counter, PSF_SEEK_SET); 
	
	do {

		player->nread = psf_sndReadFloatFrames(
			player->sfd, player->buf, FRAME_BLOCK_LEN
		);  
		Pa_WriteStream(
			player->stream, player->buf, FRAME_BLOCK_LEN
		);

	} while(player->nread == FRAME_BLOCK_LEN);
 
	psf_sndClose(player->sfd); 
	psf_finish();

	player->err = closeStream(player); 
	dlog("PHAEDRA", "Closed audio file stream");  

}


void pauseFile(player_t *player, char *filePath){
	if(player->playing == false){
		Pa_StopStream(player->stream); 
	} 
}


int isFull(queue_t* queue){
	return (queue->itemCount == queue->capacity); 
}

int isEmpty(queue_t* queue){
	return (queue->itemCount == 0); 
}


queue_t *initQueue(unsigned setCapacity){

	// set attributes
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t)); 
	queue->capacity = setCapacity; 
	queue->frontIndex = queue->itemCount = 0; 
	queue->rearIndex = setCapacity - 1; 

	// allocate space for queue
	queue->items = malloc(queue->capacity * sizeof(audionode_t*)); 
	for(int i = 0; i < queue->capacity; i++){
		queue->items[i] = malloc(sizeof(audionode_t)); 
	}

	return queue; 
} 


audionode_t *createNode(char *filePath){
	audionode_t *temp = (audionode_t*)malloc(sizeof(audionode_t)); 
	temp->filePath = filePath; 
	time_t rawtime; 
	struct tm *timeinfo; 
	time(&rawtime); 
	timeinfo = localtime(&rawtime);  
	temp->currentTime = asctime(timeinfo); 
	temp->plays = 0;
	return temp;  
}



void enqueue(queue_t* queue, char *filePath){
	if(isFull(queue)){
		return; 
	}

	audionode_t *item = createNode(filePath); 
	queue->rearIndex = (queue->rearIndex + 1) % queue->capacity;
	queue->items[queue->rearIndex] = item; 
	queue->itemCount = queue->itemCount + 1;
}



void dequeue(queue_t* queue){
	if(isEmpty(queue)){
		return; 
	}

	audionode_t *item = queue->items[queue->frontIndex]; 
	queue->frontIndex = (queue->frontIndex + 1) % queue->capacity; 
	queue->itemCount = queue->itemCount - 1;
}



int front(queue_t* queue) {
	if(isEmpty(queue)){
		return INT_MIN;  
	}
	return queue->frontIndex; 
}



int rear(queue_t* queue) {
	if(isEmpty(queue)){
		return INT_MIN;  
	}
	return queue->rearIndex; 
}



void cycleQueue(queue_t* queue, player_t *player){

	dlog("PHAEDRA", "STARTING QUEUE"); 
	for(int i = queue->frontIndex; i <= queue->rearIndex; i++){
		//play(queue->items[i]->filePath, 1, 0);
		if(player->playing == true){
			playThreaded(queue->items[i]->filePath); 		
		} else {
			pauseFile(player, queue->items[i]->filePath);
		}
	}
}


void playThreaded(char *filePath){

	player_t* player = initPlayer(false);
	player->filePath = filePath; 
	player->playing = true; 

	thread_args_t *arg = (thread_args_t*)malloc(sizeof(thread_args_t)); 
	arg->player = player; 
	arg->filePath = filePath; 

	// create thread for player controller
	pthread_t controllerThread; 
	pthread_t playerThread; 
	
	pthread_create(
		&controllerThread,
		NULL, 
		playerController, 
		arg
	);
 
	// create thread for playing audio queue
	pthread_create(
		&playerThread,
		NULL, 
		play, 
		arg
	); 

	pthread_join(controllerThread, NULL); 	
	pthread_join(playerThread, NULL); 

	pthread_exit(NULL); 


}






