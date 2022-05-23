#include <portsf.h>
#include <math.h> 
#include "logging/log.h"

#define TRUE 1
#define FALSE 0


struct panpos {
	double left; 
	double right; 
}; 

typedef struct panpos PANPOS; 


PANPOS simplepan(double position){
	PANPOS pos; 

	position *= 0.5; 
	pos.left = position - 0.5; 
	pos.left = position + 0.5; 
	return pos; 
}


double maxsamp(float* buf, unsigned long blocksize){
	// maximum abs value of a sample buffer
	double absval, peak = 0.0; 
	unsigned long i; 
	for(i = 0; i < blocksize; i++){
		absval = fabs(buf[i]); 
		if(absval > peak){
			peak = absval; 
		}
	}
	return peak; 
}

int openSampleFile(PSF_PROPS *props, char *filename, int* inputFile){

	 // init portsf  
    if(psf_init()){
        printf("Unable to start porsf\n");
		return FALSE; 
    }

	// open file
    (*inputFile) = psf_sndOpen(filename, props , 0);
    if((*inputFile) < 0){
        printf("Unable to open input file\n");
		return FALSE; 
    }

	  // check if file is already in floats form
    if(props->samptype == PSF_SAMP_IEEE_FLOAT){
        printf("File is in floats form already\n");
		return FALSE; 
    }
    props->samptype = PSF_SAMP_IEEE_FLOAT;
	return TRUE; 

}


int createOutputFile(PSF_PROPS *props, char *outfile, int* ofd, psf_format outformat){

	// check if outfile extension is known 
    outformat = psf_getFormatExt(outfile);
    if(outformat == PSF_FMT_UNKNOWN){
        printf("output file has unknown format\n");
        return FALSE; 
    }

	// create output file
    props->format = outformat;
    (*ofd) = psf_sndCreate(outfile, props, 0, 0, PSF_CREATE_RDWR);
    if((*ofd) < 0){
        printf("unable to create output file \n");
		return FALSE; 
    }

	return TRUE; 
}


int allocateSampleFrames(PSF_PROPS *props, PSF_CHPEAK **peaks, float** frame){

	// allocate space for one sample frame
    (*frame) = (float*)malloc(props->chans * sizeof(float));
    if((*frame) == NULL){
        puts("No Memory lef\n");
		return FALSE; 
    }

	printf("[ALLOCATE]: Allocate space for sample frame\n"); 

    /* allocate space for PEAK info */
    *peaks = (PSF_CHPEAK*)malloc(props->chans * sizeof(PSF_CHPEAK));
    if(peaks == NULL){
        puts("No Memory\n");
		return FALSE; 
    }
	
	printf("[ALLOCATE]: Allocate PEAK INFO\n"); 

	return TRUE; 

}


int displayPeakInformation(PSF_PROPS *props, PSF_CHPEAK *peaks, int ofd){

	if(psf_sndReadPeaks(ofd, peaks, NULL) > 0){
        long i;
        double peaktime;
        printf("PEAK INFORMATION:\n");
        for(int i = 0; i < props->chans; i++){
            peaktime = (double)peaks[i].pos / props->srate;
			dlog_f("PEAK DEBUG", peaktime); 
            printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
        }
		return TRUE; 
    }
	return FALSE; 
}



void debug_psf(PSF_PROPS *props){
	dlog_int("SAMPLE RATE", props->srate); 
	dlog_int("CHANNELS", props->chans); 
}


int main(int argc, char **argv){
	
	PSF_PROPS props;
	long framesread, totalread;
    int inputFile = -1, outputFile = -1;
 	psf_format outformat = PSF_FMT_UNKNOWN;
	PSF_CHPEAK* peaks = NULL; 
	float* frame = NULL; 

	
	debug_psf(&props); 

	int openFile = openSampleFile(&props, "sample.wav", &inputFile);
	if(openFile){
		dlog("FILE", "Opened input file");  
	}

	debug_psf(&props);

	int outfileResult  = createOutputFile(&props, "output.wav", &outputFile, outformat); 
	if(outfileResult){
		dlog("OUTPUT FILE", "Created output file");  
	}

	
	dlog_float("FRAME BEFORE", frame); 
	int allocateResult = allocateSampleFrames(&props, &peaks, &frame);
	if(allocateResult){
		dlog("ALLOCATE", "Allocated sample frames");  
	}
	dlog_float("FRAME AFTER", frame);

	framesread = psf_sndReadFloatFrames(inputFile, frame, 1); 
	totalread = 0;  	
	printf("FRAME VALUE %ld\n", framesread); 
	
	while(framesread == 1){

		if(psf_sndWriteFloatFrames(outputFile, frame, 1) != 1){
            printf("Error writting to outfile\n");
            break;
        }
	
		framesread = psf_sndReadFloatFrames(outputFile, frame, 1); 
		printf("TOTAL READ: %ld", totalread); 
		totalread += 1; 	
	}
	


	/**	
	int result  = createOutputFile(&props, "output.wav", outputFile, outformat); 
	if(result){
		dlog("OUTPUT FILE", "Created output file");  
	}

	dlog_float("FRAME BEFORE", frame); 
	int allocateResult = allocateSampleFrames(&props, &peaks, frame);
	if(allocateResult){
		dlog("ALLOCATE", "Allocated sample frames");  
	}
	dlog_float("FRAME AFTER", frame); 


	// main processing loop
	totalread = 0; 
	framesread = psf_sndReadFloatFrames(inputFile, frame, 1);
	printf("RESULT: %ld\n", framesread); 
	 
	while(framesread == 1){
		
		if(psf_sndWriteFloatFrames(outputFile, frame, 1) != 1){
            printf("Error writting to outfile\n");
            break;
        }

		dlog_float("FRAME READ", frame); 
		
		totalread += 1;
	}

	if(framesread < 0){
        printf("Error reading input file, outfile is incomplete\n");
    }else{
        printf("Done %ld sample frames copied to %s\n", totalread, "output.wav");
    }

	int peakResult = displayPeakInformation(&props, peaks, outputFile); 
	if(peakResult){
		dlog("ALLOCATED", "Allocated peak frames and channels");  
	}

	*/

	 

	return 0; 
}
