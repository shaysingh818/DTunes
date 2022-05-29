#include "lib/lib.h"


int main(int argc, char **argv){

	// variables
    PSF_PROPS inprops, outprops;
    long framesread, totalread;
    int inputFile = -1, outputFile = -1;
    psf_format outformat = PSF_FMT_UNKNOWN;
    PSF_CHPEAK* peaks = NULL;
	float* inframe = NULL; 
	float* outframe = NULL; 
    float* frame = NULL;
    double dbval, pos, inpeak = 0.0;
    float ampfac, scalefac;

	// check supplied arguments	
	if(argc < 2){
        printf("Not enough args supplied\n");
        return 1;
    }

	// check pan position argument
	pos = atof(argv[1]);
	if((pos < 1.0) || (pos > 1.0)){
		printf("Error: panpos value out of range\n"); 
	}


	// init portsf  
    if(psf_init()){
        printf("Unable to start porsf\n");
        return 1;
    }

	// open input file
    int openFile = openSampleFile(&inprops, "sample.wav", &inputFile);
    if(openFile){
        dlog("FILE", "Opened input file");
    }


	// check if infile is mono
	if(inprops.chans != 1){
		printf("Infile must be mono\n"); 
	}

	// copy PSF props
	outprops = inprops; 
	outprops.chans = 2; 

	// read frame and create panposition struct
	PANPOS thispos; 
	framesread = psf_sndReadFloatFrames(inputFile, inframe, 1); 
	totalread = 0; 
	
	// allocate memory before opening output file
	outframe = (float *)malloc(framesread * outprops.chans * sizeof(float)); 
	if(outframe == NULL){
		puts("No memory\n");
	}


	// create output file
    int outfileResult  = createOutputFile(&outprops, "output.wav", &outputFile, outformat);
    if(outfileResult){
        dlog("OUTPUT FILE", "Created output file");
    }


	// perform main processing loop
	while((framesread = psf_sndReadFloatFrames(inputFile, inframe, framesread) > 0)){
		int i, out_i; 
		for(i = 0, out_i = 0; i < framesread; i++){
			// pan left and right
			outframe[out_i++] = (float)(inframe[i]*thispos.left); 	
			outframe[out_i++] = (float)(inframe[i]*thispos.right); 
		}

		if(psf_sndWriteFloatFrames(outputFile, outframe, framesread) != framesread){
			printf("Error writting to outfile\n"); 
		}
	}

	 // cleanup
    exit:
        if(inputFile >= 0){
            psf_sndClose(inputFile);
        }

        if(outputFile >= 0){
            psf_sndClose(outputFile);
        }

        if(inframe){
            free(inframe);
        }

        if(outframe){
            free(outframe);
        }

        if(peaks){
            free(peaks);
        }

        psf_finish();
	

}
