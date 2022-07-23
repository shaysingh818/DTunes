#include "endpoints/endpoints.h"
#include "phaedra/phaedra_endpoints.h"
#include "collection/collection_endpoints.h"

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


void printDTunesBanner(FILE *fptr){
    char read_string[128];
    while(fgets(read_string,sizeof(read_string),fptr) != NULL){
        printf("%s",read_string);
	}
}


int main(int argc, char* argv[]){

	
	endpoint_t *head = NULL;

	char *filename = "banner.txt"; 
	FILE *fptr = NULL; 

	if((fptr = fopen(filename, "r")) == NULL){
		printf("Banner file doesn't exist\n"); 
		return 1; 
	}
		
	printDTunesBanner(fptr); 

	// argument debugger
	if(DEBUG == 1){
		printf("Arg 0: %s\n", argv[0]); 
		printf("Arg 1: %s\n", argv[1]); 
		printf("Arg 2: %s\n", argv[2]);
	}

	// configure endpoints
	collectionEndpoints(&head);
	phaedraEndpoints(&head);  

	if(strcmp(argv[1], "help") == 0){
		printEndpoints(head); 
	}

	execEndpoints(argc, argv, head);
	printf("\n");  

	


	return 0; 
}	
