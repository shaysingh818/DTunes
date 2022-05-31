#include "../include/wrapper/wrapper.h"

#define NFRAMES (1024)

int main(int argc, char **argv){
	
	// portsf variables
    PSF_PROPS inprops, outprops;
	PANPOS thispos; 	
    psf_format outformat = PSF_FMT_UNKNOWN;

	// new variables
	unsigned long nframes = NFRAMES; 
	long framesread, totalread; 
	int ifd = -1, ofd = -1; 
	double pos;

	float* inframe = NULL; 
	float* outframe = NULL; 

	if(argc < 2){
		dlog("USAGE", "./sfpan infile panpos"); 
		return 1; 
	}

	if(pos >= -1 && pos <= 1.0){
		dlog("MESSAGE", "PAN POSITION IS BETWEEN SPEAKERS"); 
	}else{
		dlog("ERROR", "Pan position not in between speakers"); 
		return 1; 
	}

	// init portsf  
    if(psf_init()){
        printf("Unable to start portsf\n");
        return 1;
    }

	// open input file
    int openFile = sfOpen(&inprops, argv[1], &ifd);
    if(openFile){
        dlog("FILE", "Opened input file");
    }

	outprops = inprops; 
	outprops.chans = 2; 

	// create output file
    int outfileResult  = sfOutFile(&outprops, "outsound2.wav", &ofd, outformat);
    if(outfileResult){
        dlog("OUTPUT FILE", "Created output file");
    }

	// get peak information 
    int allocateResult = sfAllocate(&inprops, &inframe);
    if(allocateResult){
        dlog("ALLOCATE", "Allocated sample frames");
    }

	totalread = 0; 
	framesread = psf_sndReadFloatFrames(ifd, inframe, 1); 	
	while(framesread == 1){
		// tracking vars
		totalread++; 
		int i, out_i; 
		// copy panning frames
		for(i = 0, out_i = 0; i < framesread; i++){
			inframe[i] = inframe[i]*thispos.left; 
			inframe[i] = inframe[i]*thispos.left; 
		}

		 // check if there's an error writting to output
        if(psf_sndWriteFloatFrames(ofd, inframe, 1) != 1){
            dlog("ERROR", "Writing to output file");
            break;
        }

		framesread = psf_sndReadFloatFrames(ifd, inframe, 1);
	}

	
    exit:
		// clear input/output files
        if(ifd >= 0){ psf_sndClose(ifd);}
        if(ofd >= 0){ psf_sndClose(ofd);}

		// clear input/output frames
        if(inframe){ free(inframe); }
        if(outframe){ free(outframe); }
	
		// deinit portsf
        psf_finish();


	return 0; 
}
