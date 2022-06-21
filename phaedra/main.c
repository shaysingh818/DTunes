#include "endpoints/endpoints.h"
#include "phaedra_endpoints.h" 
#include <stdio.h> 
#include <stdlib.h> 

int main(int argc, char **argv){

	if(argc < 2){
		printf("Supply more than 2 args\n"); 
		return 0; 
	}

	endpoint_t *head = NULL; 
	phaedraEndpoints(&head); 

	// help menu
	if(strcmp(argv[1], "help") == 0){
        printEndpoints(head);
    }

	execEndpoints(argc, argv, head); 
	printf("\n"); 

	return 0; 
	
}
