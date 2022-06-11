#include <stdio.h>
#include <math.h>
#include <portsf.h>
#include "../include/logging/log.h"

#define SAMPLING_RATE 44100
#define PI 3.14159265
#define TABLE_LEN 512
#define SINE 0
#define SQUARE 1
#define SAW 2 
#define TRIANGLE 3


// global var
float table[TABLE_LEN]; 


void fill_sine(){
	for(int i = 0; i < TABLE_LEN; i++){
		table[i] = (float) sin(2 * PI * i/TABLE_LEN); 
	}
}


void fill_square(){
	for(int i = 0; i < TABLE_LEN/2; i++){
		table[i] = 1; 
	}

	for(int i = 0; i < TABLE_LEN; i++){
		table[i] = -1; 
	}
}


void fill_saw(){
	for(int i = 0; i < TABLE_LEN; i++){
		table[i] = 1 - (2 * (float) i / (float) TABLE_LEN); 
	}
}


void fill_triangle(){
	for(int i = 0; i < TABLE_LEN/2; i++){
		table[i] = 2 * (float) i / (float) (TABLE_LEN/2) - 1; 
	}
	
	for(int i = TABLE_LEN/2; i < TABLE_LEN; i++){
		table[i] = 1 - (2 * (float) (i-TABLE_LEN/2) / (float) (TABLE_LEN/2)); 
	}
}


void outSample(int* ofd, float sample){
	psf_sndWriteFloatFrames((*ofd), &sample, 1); 
}

void init(int* ofd, PSF_PROPS* props){
	
	props->srate = 44100;
	props->chans = 1; 
	props->samptype = PSF_SAMP_16; 
	props->format = PSF_STDWAVE;
	props->chformat = STDWAVE; 

	// init portsf
    if(psf_init()){
        printf("Unable to start portsf\n");
        return;
    }

	(*ofd) = psf_sndCreate("output.wav", props, 0, 0, PSF_CREATE_RDWR); 
	

}


int main(int argc, char **argv){

	// portsf vars
	PSF_PROPS props; 
	int ofd; 

	int waveform;
	const float frequency, duration; 
	
	printf("Type the frequency of the output to the wav file in Hz\n");
	scanf("%f", &frequency);
 
	printf("Type the duration of the wav file\n");
	scanf("%f", &duration); 

	wrong_waveform:
		printf("Type number from 0 to 3 for waveform\n");
		scanf("%d", &waveform);

	if(waveform < 0 || waveform > 3){
		printf("Wrong number for wave form\n"); 
		goto wrong_waveform; 
	}

	switch(waveform){

		case SINE: 
			dlog("OPTION", "SINE");
			fill_sine();  
			break; 

		case SQUARE: 
			dlog("OPTION", "SQUARE"); 
			fill_square(); 
			break; 

		case SAW: 
			dlog("OPTION", "SAW");
			fill_saw();  
			break; 
		
		case TRIANGLE: 
			dlog("OPTION", "TRIANGLE");
			fill_triangle(); 
			break; 

		default: 
			dlog("ERROR", "WRONG WAVE FORM CHOSEN"); 
			return 0;  
	}

	init(&ofd, &props); 

	/* SYNTHESIS ENGINE START */ 
	{
		double sample_increment = frequency * TABLE_LEN / SAMPLING_RATE; 
		double phase = 0; 
		float sample; 

		for(int j = 0; j < duration * SAMPLING_RATE; j++){
			sample = table[(long) phase]; 
			outSample(&ofd, sample);
		}

		phase += sample_increment; 
		if(phase > TABLE_LEN) { phase -= TABLE_LEN; }; 

	}


	/* CLEANUP */ 
	int err1, err2; 
	err1 = psf_sndClose(ofd); 
	err2 = psf_finish(); 
	
	if(err1 || err2){
		dlog("ERROR", "ERROR CLOSING PORTSF"); 
	}
		
}




