#include <stdio.h> 
#include <math.h> 

#define SAMPLING_RATE 44100
#define NUM_SECONDS 3
#define NUM_SAMPLES (NUM_SECONDS * SAMPLING_RATE)
#define PI 3.14159265
#define FREQ 440

int main(int argc, char **argv){
	for(int j = 0; j < NUM_SAMPLES; j++){
		float sample; 
		sample = sin(2 * PI * FREQ * j / SAMPLING_RATE); 
		printf("%f\n", sample);  
	}
}
