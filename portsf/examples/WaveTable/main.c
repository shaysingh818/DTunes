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



	
// 	/* Check for output files */ 
// 	if(output == NULL){
// 		printf("No output file was given\n"); exit(1); 
// 	}

// 	if(wavename == NULL && formula == -1){
// 		printf("Formula and wave form is not specified\n"); 
// 	}
	
// 	if(wavename != NULL && formula != -1){
// 		printf("Formula and waveform supplied\n"); 
// 	}

// 	if(wavename == NULL) return FORMULA_GIVEN; 
// 	else return WAVEFORM_GIVEN;
// }

ARG_RES decode_args(int argc, char **argv){
	for(int i = 0; i < argc; i++){
		char *currentArg = argv[i];
		// get first index of char pointer 
		char firstIndex = currentArg[0];
		if(firstIndex == '-'){
			printf("Tag Argument: %s\n", &currentArg[1]); 
			// set variables based on tag arguments
			char current = currentArg[1]; 
			switch(current){
				case 'w':
					if(argc == 0) printf("No wave argument supplied"); 
				    wavename = argv[i+1];
					break; 
				case 'f':
					if(strcmp(argv[i+1], "sine") == 0){
						formula = SINE; 
					}else if(strcmp(argv[i+1], "cosine") == 0){
						formula = COSINE; 
					}else{
						printf("Invalid formula name\n"); 
					}
					
					printf("FORMULA %d\n", formula); 
					break; 
				case 's':	
					speed = atof(argv[i+1]); 
					printf("Speed set: %f\n", speed); 
				case 't':	
					duration = atof(argv[i+1]);
					printf("Duration set: %f\n", duration); 	
					break; 
				case 'o':	
					output = argv[i+1];	
					break;
				default: 
					printf("Hitting default case\n");  
			}
		}
	}


	if(output == NULL) {
		printf("No output file given\n"); 
		exit(1); 
	}
	if(wavename == NULL && formula == -1){
		printf("No waveform or formula specified\n"); 
		exit(1); 
	}

	if(wavename != NULL && formula != -1){
		printf("Both waveform and formula specified\n"); 
		exit(1); 
	}
	if(wavename == NULL) return FORMULA_GIVEN; 
	else return WAVEFORM_GIVEN; 
} 




int main(int argc, char **argv){
	
	int format = decode_args(argc, argv);	
	printf("Formula: %d\n", formula); 
	OUTPUT ff; 
	WAVEFORM xx; 
	psf_init(); 
	if(format == WAVEFORM_GIVEN){
		xx = readform(wavename); 
	}else{
		xx = createformula(formula); 
	}
	printf("Output: %s\n", output); 
	ff = init_sound(output, sr); 	
	copywaveform(xx, speed, ff, duration); 
	close_sound(ff); 
	return 0; 

}
