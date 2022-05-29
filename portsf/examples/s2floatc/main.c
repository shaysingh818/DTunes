#include <portsf.h>
#include <stdio.h> 
#include <stdlib.h> 


#define TRUE 1
#define FALSE 0


// function to check sample rate
int checkSampleType(psf_stype type){
	int accept = 1; 
	switch(type){
		case(PSF_SAMP_8):
			printf("8 bit sample rate\n"); 
			accept = 0; 
			break; 
		case(PSF_SAMP_16):
			printf("16 bit sample\n");
			break; 
		case(PSF_SAMP_24):
			printf("24 bit sample rate\n"); 
			break; 
		case(PSF_SAMP_32):
			printf("32 bit sample rate\n"); 
			break; 
		case(PSF_SAMP_IEEE_FLOAT):
			printf("32 bit floating point \n");
			break; 
		default:
			printf("Unknown\n"); 
			accept = 0; 
	}

	return accept; 
}


// display sample rate in wav file
int displaySampleRate(char *filename){
	PSF_PROPS props;
	int sf;
	sf = psf_sndOpen(filename, &props,0);
	if(sf < 0){
		printf("Error: unable to open soundfile\n");
		return 1;
	}
	printf("Sample rate = %d\n", props.srate);
	printf("number of channels = %d\n", props.chans);
	return 0;
}


int testFunction(){
	int ofd; 
	PSF_PROPS props; 

	
	psf_format format; 
	format = psf_getFormatExt("soundtrack.wav");

	props.srate = 96000; 
	props.chans = 6; 
	props.samptype = PSF_SAMP_24; 
	props.format = format; 
	props.chformat = MC_DOLBY_5_1; 

	ofd = psf_sndCreate("soundtrack.wav", &props, 1, 0, PSF_CREATE_RDWR); 
	if(ofd < 0){
		printf("Error: unable to create output file"); 
		return 1; 
	}

	return 0; 
}


void convertToFloat(char *filename, char *outfile){

	PSF_PROPS props; 
	long framesread, totalread;
	int inputFile = -1, outputFile = -1;
	int error = 0; 
	psf_format outformat = PSF_FMT_UNKNOWN; 
	PSF_CHPEAK* peaks = NULL; 
	float* frame = NULL;  

	// init portsf	
	if(psf_init()){
		printf("Unable to start porsf\n");
	}

	// open file
	inputFile = psf_sndOpen(filename, &props , 0);
	if(inputFile < 0){
		printf("Unable to open input file\n"); 
	}

	// check if file is already in floats form
	if(props.samptype == PSF_SAMP_IEEE_FLOAT){
		printf("File is in floats form already\n"); 
	}
	props.samptype = PSF_SAMP_IEEE_FLOAT;

	// check if outfile extension is known 
	outformat = psf_getFormatExt(outfile); 
	if(outformat == PSF_FMT_UNKNOWN){
		printf("output file has unknown format\n"); 	
		goto exit; 
	}

	// create output file
	props.format = outformat; 
	outputFile = psf_sndCreate(outfile, &props, 0, 0, PSF_CREATE_RDWR); 
	if(outputFile < 0){
		printf("unable to create output file \n"); 
		goto exit; 
	}

	//printf("FRAME BEFORE %f\n", frame); 
	// allocate space for one sample frame
	frame = (float*)malloc(props.chans * sizeof(float)); 
	if(frame == NULL){
		puts("No Memory lef\n"); 
		goto exit; 
	}

	//printf("FRAME AFTER %ld\n", frame); 

	/* allocate space for PEAK info */
	peaks = (PSF_CHPEAK*)malloc(props.chans * sizeof(PSF_CHPEAK)); 
	if(peaks == NULL){
		puts("No Memory\n"); 
		goto exit; 
	}

	printf("Copying.....\n"); 

	// single frame copy, report any errors if any 
	framesread = psf_sndReadFloatFrames(inputFile, frame, 1);
	totalread = 0;
	printf("FRAME VALUE %ld\n", framesread);  
	while(framesread == 1){
		totalread++;
		if(psf_sndWriteFloatFrames(outputFile, frame, 1) != 1){
			printf("Error writting to outfile\n"); 
			break; 
		}

		// do any processing here
		printf("FRAME READ: %ld\n", totalread); 
		framesread = psf_sndReadFloatFrames(inputFile, frame, 1); 	

	}

	if(framesread < 0){
		printf("Error reading input file, outfile is incomplete\n"); 
	}else{
		printf("Done %ld sample frames copied to %s\n", totalread, outfile); 
	}

	if(psf_sndReadPeaks(outputFile, peaks, NULL) > 0){
		long i; 
		double peaktime; 
		printf("PEAK INFORMATION:\n"); 
		for(int i = 0; i < props.chans; i++){
			peaktime = (double)peaks[i].pos / props.srate; 
			printf("PEAK TIME: %f\n", peaktime); 
			printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime); 
		}
	}

	// do all the cleanup
	exit:
		if(inputFile >= 0){
			psf_sndClose(inputFile); 
		}

		if(outputFile >= 0){
			psf_sndClose(outputFile); 
		}
	
		if(frame){
			free(frame); 
		}

		if(peaks){
			free(peaks); 
		}
		psf_finish(); 

		// return error; 

}



int main(int argc, char **argv){

	if(argc < 2){
		printf("Not enough args supplied\n"); 
		return 1; 
	}

	// execute function (put in endpoint at some time)
	convertToFloat(argv[1], argv[2]); 

	return 0; 
	

}
