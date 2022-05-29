#include "lib/lib.h"



int main(int argc, char **argv){
	
	// variables
	PSF_PROPS props;
	long framesread, totalread;
    int inputFile = -1, outputFile = -1;
 	psf_format outformat = PSF_FMT_UNKNOWN;
	PSF_CHPEAK* peaks = NULL; 
	float* frame = NULL; 
	double dbval, pos, inpeak = 0.0; 
	float ampfac, scalefac; 

	if(argc < 2){
		printf("Not enough args supplied\n");
		return 1; 
	}

	pos = (atof(argv[1])); 
	if(pos < -1.0 || pos > 1.0){
		printf("pan position incorrect\n "); 
		return 1; 
	}

	 // init portsf  
    if(psf_init()){
        printf("Unable to start porsf\n");
		return 1; 
    }

	
	// debug_psf(&props); 

	// open file
	int openFile = openSampleFile(&props, "sample.wav", &inputFile);
	if(openFile){
		dlog("FILE", "Opened input file");  
	}


	// create output file
	int outfileResult  = createOutputFile(&props, "output.wav", &outputFile, outformat); 
	if(outfileResult){
		dlog("OUTPUT FILE", "Created output file");  
	}


	// get peak information	
	int allocateResult = allocateSampleFrames(&props, &peaks, &frame);
	if(allocateResult){
		dlog("ALLOCATE", "Allocated sample frames");  
	}

	// read frame
	framesread = psf_sndReadFloatFrames(inputFile, frame, 1); 
	totalread = 0;  	
	

	// main loop
	while(framesread == 1){

		if(psf_sndWriteFloatFrames(outputFile, frame, 1) != 1){
            printf("Error writting to outfile\n");
            break;
        }

		framesread = psf_sndReadFloatFrames(inputFile, frame, 1);
		totalread += 1;  
	
	}


	// check file read status
	if(framesread < 0){
        printf("Error reading input file, outfile is incomplete\n");
    }else{
        printf("Done %ld sample frames copied to %s\n", totalread, "output.wav");
    }

	
	// display peak information
	int peakResult = displayPeakInformation(&props, peaks, &outputFile); 
	if(peakResult){
		dlog("ALLOCATED", "Allocated peak frames and channels");  
	}

	
	// cleanup
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


	return 0; 
}
