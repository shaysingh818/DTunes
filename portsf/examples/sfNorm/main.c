#include "../include/lib/lib.h"

// number of frames without loop over
#define NFRAMES (1024)



int main(int argc, char **argv){

	// portsf vars
	PSF_PROPS props;
	PSF_CHPEAK* peaks = NULL; 
	psf_format outformat = PSF_FMT_UNKNOWN;  
	
	// input/output vars
	long framesread, totalread; 
	int ifd = -1, ofd = -1; 
	float* frame = NULL; 
	unsigned long nframes = NFRAMES; 

	// normalization variables
	double dbval, inpeak = 0.0; 
	float ampfac, scalefac; 

	

	// check supplied arguments 
    if(argc < 4){
		dlog("ARGUMENT ERROR", "Not enough args supplied"); 
		dlog("USAGE", "./sfnorm infile dbfactor"); 
        return 1;
    }

	// read ampfac from command line
    dbval = atof(argv[2]);
    if(dbval > 0.0){
        dlog("ERROR", "DB VAL cannot be positive");
        return 1;
    }

	// calculate ampfactor based on db val
	ampfac = (float)pow(10.0, dbval/20.0); 

	// init portsf  
    if(psf_init()){
        printf("Unable to start porsf\n");
        return 1;
    }

	// open input file
    int openFile = openSampleFile(&props, "sample.wav", &inputFile);
    if(openFile){
        dlog("FILE", "Opened input file");
    }

	// get peak information 
    int allocateResult = allocateSampleFrames(&props, &peaks, &frame);
    if(allocateResult){
        dlog("ALLOCATE", "Allocated sample frames");
    }

	frame = (float*)malloc(NFRAMES * props.chans * sizeof(float)); 

	framesread = psf_sndReadFloatFrames(ifd, frame, nframes); 
	while(framesread > 0){
		double thispeak; 
		blocksize = framesread * props.chans; 
		thispeak = maxsamp(frame, blocksize); 
		if(thispeak > inpeak){
			inpeak = thispeak; 
		}

		// next stage
		if(psf_sndReadPeaks(ifd, peaks, NULL) > 0){
			long i; 
			for(i = 0; i < props.chans; i++){
				if(peaks[i].val > inpeak){
					inpeak = peaks[i].val; 
				}
			}
		}else{
			// scan the file and rewind
			if((psf_sndSeek(ifd, 0, PSF_SEEK_SET)) < 0){
				dlog("ERROR", "UNABLE TO REWIND FILE"); 
				goto exit; 
			}
		}

		
		// check if infile is silent
		if(inpeak == 0.0){
			dlog("WARNING", "INFILE IS SILENT, OUTFILE NOT CREATED"); 
			goto exit; 
		}

		framesread = psf_sndReadFloatFrames(ifd, frame, nframes); 
	}


	// calculate scale factor
	scalefac = (float)(ampfac / inpeak); 
	
	// create output file
    int outfileResult  = createOutputFile(&props, "outsound2.wav", &outputFile, outformat);
    if(outfileResult){
        dlog("OUTPUT FILE", "Created output file");
    }

	


	
}


