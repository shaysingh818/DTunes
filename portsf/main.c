#include <portsf.h>
#include <stdio.h> 
#include <stdlib.h> 


#define TRUE 1
#define FALSE 0

// function init porsf
int initPortsf(){
	return TRUE; 
}


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
	int inputFile = -1, outputFile = -1
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
		
	}

	

}



int main(){


}
