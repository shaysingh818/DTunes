#include "wrapper.h"

PANPOS simplepan(double position){
    PANPOS pos;
    position *= 0.5;
    pos.left = position - 0.5;
    pos.left = position + 0.5;
    return pos;
}


PANPOS constantpower(double position){
	PANPOS pos; 
	const double piovr2 = 4.0 * atan(1.0) * 0.5; 
	const double root2ovr2 = sqrt(2.0) * 0.5; 
	double thispos = position * piovr2; 
	// each channel uses a 1/4 of a cycle
	double angle = thispos * 0.5; 
	// adjust positions
	pos.left = root2ovr2 * (cos(angle) - sin(angle)); 
	pos.right = root2ovr2 * (cos(angle) + sin(angle)); 
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


int sfOpen(PSF_PROPS *props, char *filename, int* ifd){	
    // open file
    (*ifd) = psf_sndOpen(filename, props , 0);
    if((*ifd) < 0){
		dlog("SF ERROR", "Unable to open input file"); 
		return FALSE; 
    }
    // check if file is already in floats form
    if(props->samptype == PSF_SAMP_IEEE_FLOAT){
       	dlog("SF ERROR", "File is in floats form already\n");
    }

	return TRUE; 
}


int sfOutFile(PSF_PROPS *props, char *filename, int* ofd, psf_format outformat){

    outformat = psf_getFormatExt(filename);
    if(outformat == PSF_FMT_UNKNOWN){
        dlog("SF ERROR", "output file has unknown format\n");
    }
    // create output file
    props->format = outformat;
    (*ofd) = psf_sndCreate(filename, props, 0, 0, PSF_CREATE_RDWR);
    if((*ofd) < 0){
        dlog("SF ERROR", "unable to create output file \n");
		return FALSE; 
    }

	return TRUE; 
}


int sfAllocate(PSF_PROPS *props, float **frames){

    (*frames) = (float*)malloc(props->chans * sizeof(float));
    if((*frames) == NULL){
        puts("No Memory lef\n");	
		return FALSE; 
    }
	return TRUE; 

}


int sfAllocateSize(PSF_PROPS *props, float **frames, unsigned long nframes){

    (*frames) = (float*)malloc(nframes * props->chans * sizeof(float));
    if((*frames) == NULL){
        puts("No Memory lef\n");	
		return FALSE; 
    }
	return TRUE; 

}

int sfAllocatePeaks(PSF_PROPS *props, PSF_CHPEAK **peaks){

    /* allocate space for PEAK info */
    *peaks = (PSF_CHPEAK*)malloc(props->chans * sizeof(PSF_CHPEAK));
    if(peaks == NULL){
		dlog("SF ERROR", "No Memory Peaks"); 
		return FALSE; 
    }
	return TRUE; 
}



void sfPeakInfo(PSF_PROPS *props, PSF_CHPEAK *peaks, int* ofd){

    if(psf_sndReadPeaks(*ofd, peaks, NULL) > 0){
        long i;
        double peaktime;
        printf("PEAK INFORMATION:\n");
        for(int i = 0; i < props->chans; i++){
            peaktime = (double)peaks[i].pos / props->srate;
            dlog_f("PEAK DEBUG", peaktime);
            printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
        }
    }
}


void debug_psf(PSF_PROPS *props){
    dlog_int("SAMPLE RATE", props->srate);
    dlog_int("CHANNELS", props->chans);
}


