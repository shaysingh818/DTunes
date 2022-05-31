#ifndef WRAPPER_H
#define WRAPPER_H

#include <portsf.h>
#include <math.h>
#include "../logging/log.h"

#define TRUE 1
#define FALSE 0


typedef struct panpos {
    double left;
    double right;
} PANPOS;


// helper functions 
PANPOS simplepan(double position);
double maxsamp(float* buf, unsigned long blocksize);

// portsf wrapper functions
int sfOpen(PSF_PROPS *props, char *filename, int* ifd); 
int sfOutFile(PSF_PROPS *props, char *filename, int* ofd, psf_format outformat); 
int sfAllocate(PSF_PROPS *props, float** frame);
int sfAllocateSize(PSF_PROPS *props, float** frame, unsigned long nframes);
int sfAllocatePeaks(PSF_PROPS *props, PSF_CHPEAK **peaks); 
void sfPeakInfo(PSF_PROPS *props, PSF_CHPEAK *peaks, int* ofd);

// debugging portsf
void debug_psf(PSF_PROPS *props);

#endif
