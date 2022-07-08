#include "phaedra_endpoints.h"


int main(int argc, char* argv[]){
	
	endpoint_t *head = NULL; 
	
	phaedraEndpoints(&head); 

	if(strcmp(argv[1], "help") == 0){
        printEndpoints(head);
    }

    execEndpoints(argc, argv, head);
    printf("\n");

}
