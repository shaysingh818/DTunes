#include "waveform.h"
#include <portsf.h> 

int length_of_file(FILE_HANDLE file){
	FILE *ff = fopen(file, "r"); 
	int count; 
	if(ff == NULL) return 0; 
	if(fscanf(ff, "%d", &count) != 1){
		fclose(ff); 
		return 0; 
	}
	fclose(ff); 
	return count; 
}


int readfile(WAVEFORM xx, FILE_HANDLE file){
	int count, i; 
	double val; 
	FILE *ff = fopen(file, "r"); 
	if(ff == NULL) return ERR_NO_FILE; 
	if(fscanf(ff, "%d", &count) != 1) return ERR_NO_COUNT; 
	for(i = 0; i < count; i++){
		if(fscanf(ff, "%lf", &val) != 1) return ERR_NO_DATA; 
		put_val(xx, i, val); 
	}
	fclose(ff); 
	return 0; 
}


WAVEFORM readform(FILE_HANDLE file){
	WAVEFORM xx; 
	int err; 
	xx = getnewform(length_of_file(file)); 
	if((err=readfile(xx, file)) == 0) return xx; 
	printf("Failed to read wave form for reason %d", err); 
	return NULL; 
}


WAVEFORM createformula(FORMULA trig){
	WAVEFORM xx = getnewform(TRIG_SIZE); 
	double val; 
	for(int i = 0; i < TRIG_SIZE; i++){
		if(trig == SINE) {val = sin((double)i*TWOPIBYSIZE);} 
		else if  (trig == COSINE) { val = cos((double)i*TWOPIBYSIZE);}
		else { return NULL; }; 
		put_val(xx, i, val);  
	}
	return xx; 
}

WAVEFORM getnewform(int size){
	WAVEFORM xx = (WAVEFORM)malloc((size-1)*sizeof(double)+sizeof(WAVE)); 
	if(xx == NULL){
		printf("Failed to allocate memory for wave\n"); 
		exit(1); 
	}
	return xx; 
}

void put_val(WAVEFORM xx, int i, double val){
	xx->samples[i] = val; 
}


void copywaveform(WAVEFORM w, SPEED s, OUTPUT out, double T){
	double index = 0.0; 
	int total_samps = (int)(T*sr); 
	int size = waveform_size(w); 
	for(int i = 0; i < total_samps; i++){
		double value = waveform_val(w, (int)index); 
		index += size*s/sr;
		if(index >= size) index -= size; 
		output_sample(out, value); 
	}
}


int waveform_size(WAVEFORM w){
	return w->size; 
}

double waveform_val(WAVEFORM w, int which){
	return w->samples[which];
}


double waveform_ival(WAVEFORM w, double which){
	int x0 = (int)which; 
	int x1 = (int)(which+1.0); 
	double f0, f1; 
	if(x1 >= waveform_size(w)) x1 = 0; 
	f0 = waveform_val(w, x0); 
	f1 = waveform_val(w, x1); 
	return f0 + (f1 - f0) * (which-x0); 
}


/* Functions for OUTPUT */ 

OUTPUT init_sound(char *name, int srate){
	PSF_PROPS inprops; 
	inprops.srate = (long)srate; 
	inprops.chans = 1L; 
	inprops.samptype = PSF_SAMP_16; 
	inprops.format = PSF_STDWAVE; 
	inprops.chformat = MC_MONO; 
	return psf_sndOpen(name, &inprops, 0); 
}


int output_sample(OUTPUT out, double sample){
	short buffer[1]; 
	buffer[0] = (short)(sample*32767.0); 
	if(psf_sndWriteShortFrames(out, buffer, 1) != 1L){
		return 0; 
	}
	return 1; 
}

int close_sound(OUTPUT out){
	psf_sndClose(out); 
	return 1; 
}




void errormsg(char *format, enum ERRORS err){
	fprintf(stderr, format, (int)err); 
	switch(err){
		case ERR_NO_FILE:
			printf("\nfile not found\n"); 
			return;
		 
		case ERR_NO_COUNT:
			printf("\nfile not found\n"); 
			return;
 
		case ERR_NO_DATA:
			printf("\nfile not found\n"); 
			return; 
	
		case ERR_NO_FORMULA:
			printf("\nfile not found\n"); 
			return; 
	
		case ERR_NO_MEMORY:
			printf("\nfile not found\n"); 
			return; 
	}
}


