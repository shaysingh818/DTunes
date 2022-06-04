#include "../include/wrapper/wrapper.h"

#define NFRAMES (1024)

int main(int argc, char **argv){

	// portsf variables
    PSF_PROPS inprops, outprops;	
    psf_format outformat = PSF_FMT_UNKNOWN;
    PSF_CHPEAK* peaks = NULL;

	// frame variables
    long framesread, totalread;
	unsigned long nframes = NFRAMES; 
    int inputFile = -1, outputFile = -1;
	float* inframe = NULL;
	float* outframe = NULL; 
	float* frame = NULL; 
    double pos;

	// ampfactor
	float ampfac; 
	
	// check supplied arguments	
	if(argc < 2){
        printf("Not enough args supplied\n");
        return 1;
    }

	// read ampfac from command line
	ampfac = (float) atof(argv[2]); 
	if(ampfac <= 0.0){
		dlog("ERROR", "AMP FAC MUST BE POSITIVE"); 
		return 1; 
	}


	// init portsf  
    if(psf_init()){
        printf("Unable to start porsf\n");
        return 1;
    }

	// open input file
    int openFile = sfOpen(&inprops, "sample.wav", &inputFile);
    if(openFile){
        dlog("FILE", "Opened input file");
    }

	
	debug_psf(&inprops); 


	// create output file
    int outfileResult  = sfOutFile(&inprops, "outsound2.wav", &outputFile, outformat);
    if(outfileResult){
        dlog("OUTPUT FILE", "Created output file");
    }


	// get peak information 
    int allocateResult = sfAllocate(&inprops, &frame);
    if(allocateResult){
        dlog("ALLOCATE", "Allocated sample frames");
    }

		
	totalread = 0; 
	framesread = psf_sndReadFloatFrames(inputFile, frame, 1); 
	// perform main processing loop
	while(framesread ==  1){
		totalread++;

		// increase level
		for(int i = 0; i < inprops.chans; i++){
			frame[i] *= ampfac; 
		}

		// check if there's an error reading a frame
		if(psf_sndWriteFloatFrames(outputFile, frame, 1) != 1){
			dlog("ERROR", "Writing to output file"); 
			break; 
		}

		framesread = psf_sndReadFloatFrames(inputFile, frame, 1); 
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
	

}
