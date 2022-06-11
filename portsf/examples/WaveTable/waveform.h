#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 

enum FORUMLAE {SINE, COSINE};
typedef enum FORUMLAE FORMULA; 

typedef struct WaveForm {
	int size; 
	double samples[1]; 
} WAVE;

typedef WAVE *WAVEFORM; 

typedef char *FILE_HANDLE;
typedef double SPEED; 
typedef int OUTPUT; 

enum ERRORS {
	ERR_NO_FILE=100, 
	ERR_NO_COUNT, 
	ERR_NO_DATA, 
	ERR_NO_FORMULA, 
	ERR_NO_MEMORY
}; 

#define sr (44100)
#define TRIG_SIZE (4096)
#define TWOPIBYSIZE (6.28318530718/(double)TRIG_SIZE)

/* waveform functions */
int length_of_file(FILE_HANDLE file); 
int readfile(WAVEFORM xx, FILE_HANDLE file); 
WAVEFORM readform(FILE_HANDLE); 
WAVEFORM createformula(FORMULA); 
WAVEFORM getnewform(int size); 
void put_val(WAVEFORM xx, int i, double val); 
void copywaveform(WAVEFORM w, SPEED s, OUTPUT out, double T);
int waveform_size(WAVEFORM w); 
double waveform_val(WAVEFORM w, int which); 
double waveform_ival(WAVEFORM w, double which); 

/* output functions */ 
OUTPUT init_sound(char *name, int srate); 
int output_sample(OUTPUT out, double sample); 
int close_sound(OUTPUT out); 
void errormsg(char *format, enum ERRORS err); 

