#include "endpoints.h"

endpoint_t *createEndpoint(char *name, char *commandLineArg, char *docString){
    // create endpoint
    endpoint_t *newEndpoint = (endpoint_t*)malloc(sizeof(endpoint_t));
    strcpy(newEndpoint->name, name);
    strcpy(newEndpoint->commandLineArg, commandLineArg);
    newEndpoint->documentation = (char*)malloc(strlen(docString) + 1);
    strcpy(newEndpoint->documentation, docString);
    return newEndpoint;
} 


void appendEndpoint(endpoint_t **head_ref, endpoint_t *e1){
    // add endpoint in linked list
    e1->next = (*head_ref);
    (*head_ref) = e1;
}


void execEndpoints(int argc, char *argv[], endpoint_t *head_ref){
    while(head_ref != NULL){	
		 // check arg condition
        head_ref->endpointLogic(head_ref, argv);
        // go to next head reference
        head_ref = head_ref->next;
    }
}


void printEndpoints(endpoint_t *head_ref){
    // display endpoints
    char *column1 = "FLAG-COMMAND";
    char *column3 = "DOCUMENTATION";
    // display 
    printf("\e[0;31m");
    printf("%c[1m",27);
    printf("%-25s  %-25s\n", column1, column3);
	generateBanner(65); 
    while(head_ref != NULL){
        	printf("--%-20s  %-25s\n",
            head_ref->commandLineArg,
            head_ref->documentation
        );
        head_ref = head_ref->next;
    }
}
