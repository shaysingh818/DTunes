#include "waveform.h"
#include <portsf.h>
#include <string.h> 
#include <stdlib.h> 


/* program vars */ 
double duration = 3.0; 
double speed = 440.0; 
char *output = NULL;
char *wavename = NULL; 
int formula = -1; 
/* command enums */ 
enum ARG_RESE {WAVEFORM_GIVEN, FORMULA_GIVEN};
typedef enum ARG_RESE ARG_RES;


/* decode command line arguments */ 
ARG_RES decode_args(int argc, char **argv){
	int i = 0; 
	char *a = argv[0]; 
	while(--argc){
		a = argv[i]; 
		if(a[0] == '-'){  /* found tag argument */ 
			switch(a[1]){
				case 'w':
					if(argc == 0) printf("No wave argument supplied"); 
				    wavename = argv[i+1]; 
					argc--; i += 2; 
					break; 
				case 'f':	
					if(argc == 0) printf("No wave argument supplied"); 
					if(strcmp(argv[i+1], "sine") == 0){
						formula = SINE;
					}else if (strcmp(argv[i+1], "cosine") == 0){
						formula = COSINE;
					}else{
						printf("Invalid formula name\n"); 
					} 
					argc--; i += 2; 
					break; 
				case 's':
					if(argc == 0) printf("No speed given\n"); exit(1); 
					speed = atof(argv[i+1]); 
					argc--; i += 2; 

				case 't':	
					if(argc == 0) printf("No duration given\n"); exit(1); 
					duration = atof(argv[i+1]);	
					argc--; i += 2; 
					break; 
				case 'o':	
					output = argv[i+1];	
					argc--; i += 2; 
					break; 
			}
		}else{
			printf("No tag found\n"); exit(1); 
		}
	}

	
	/* Check for output files */ 
	if(output == NULL){
		printf("No output file was given\n"); exit(1); 
	}

	if(wavename == NULL && formula == -1){
		printf("Formula and wave form is not specified\n"); 
	}
	
	if(wavename != NULL && formula != -1){
		printf("Formula and waveform supplied\n"); 
	}

	if(wavename == NULL) return FORMULA_GIVEN; 
	else return WAVEFORM_GIVEN;
} 



int main(int argc, char **argv){
	
	int format = decode_args(argc, argv); 
	OUTPUT ff; 
	WAVEFORM xx; 
	psf_init(); 
	if(format == WAVEFORM_GIVEN){
		xx = readform(wavename); 
	}else{
		xx = createformula(formula); 
	}
	ff = init_sound(output, sr); 	
	copywaveform(xx, speed, ff, duration); 
	close_sound(ff); 
	return 0; 

}
