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

	debug_psf(&inprops);

	if(inprops.chans != 1){
		dlog("ERROR", "Infile must be mono"); 
		goto exit; 
	}

	int out = sfOutFile(&inprops, "outfile.wav", &ofd, outformat);
	if(out){
		dlog("OUTFILE", "Created output file"); 
	} 

	// allocate amount of frames
	int allocate = sfAllocateSize(&inprops, &inframe, nframes);
    if(allocate){
        dlog("ALLOCATE", "Allocated sample frames");
    }

	// main processing loop
	totalread = 0;
	
	framesread = psf_sndReadFloatFrames(ifd, inframe, nframes);
	while(framesread > 0){
		for(int i = 0; i < framesread; i++){
			printf("Frame value: %f\n", inframe[i]);
		}
		
		framesread = psf_sndReadFloatFrames(ifd, inframe, nframes);
	}
	
	
	/**framesread = psf_sndReadFloatFrames(ifd, inframe, 1);
	while(framesread == 1){	
		// loop through frames
		for(int i = 0; i < inprops.chans; i++){
			printf("Frame value: %f\n", inframe[i]); 
			inframe[i] *= 2;  
		}

		framesread = psf_sndReadFloatFrames(ifd, inframe,1);
	
		totalread++; 
 
	}*/



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
