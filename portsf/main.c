#include <portsf.h>
#include <stdio.h> 
#include <stdlib.h> 

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


int displaySampleRate(){
	PSF_PROPS props;
	int sf;
	sf = psf_sndOpen("sample.wav",&props,0);
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



int main(){

	PSF_PROPS props; 
	long framesread, totalread; 

	int ifd = -1, odf = -1; 
	int error = 0; 

	psf_format outformat = PSF_FMT_UNKNOWN; 
	PSF_CHPEAK* peaks = NULL; 
	float* frame = NULL; 
	
	printf("Soundfile to float format\n"); 
	
	if(psf_init()){
		printf("Unable to start portsf\n"); 
		return 1; 
	}

	ifd = psf_sndOpen("sample.wav", &props, 0); 
	if(ifd < 0){
		printf("Unable to open file\n"); 
		return 1; 
	}

	// check if file is in float form already
	if(props.samptype  == PSF_SAMP_IEEE_FLOAT){
		printf("FILE IS already in float form\n"); 
	}

	props.samptype = PSF_SAMP_IEEE_FLOAT; 
	outformat = psf_getFormatExt("soundtrack.wav"); 

	if(outformat == PSF_FMT_UNKNOWN){
		printf("Unknown format for file\n"); 
	}


	// allocate space for frames
	frame = (float*)malloc(props.chans * sizeof(float)); 
	if(frame == NULL){
		printf("No Memory\n"); 
	}

	// allocate space for PEAK info
	peaks = (PSF_CHPEAK*)malloc(props.chans * sizeof(PSF_CHPEAK)); 
	if(peaks == NULL){
		puts("No memory\n"); 
	}

	printf("Copying\n"); 

	framesread = psf_sndReadFloatFrames(ifd, frame, 1); 
	totalread = 0; 

	while(framesread == 1){
		totalread++; 
		if(psf_sndWriteFloatFrames(ofd, frame, 1) != 1){
			printf("Error writting to outfile\n"); 
			break; 
		}

	}

}
