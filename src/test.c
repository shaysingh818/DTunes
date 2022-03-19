#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>
#include <sqlite3.h>
#include "db.h"

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


struct Endpoint {
    char name[100];
    char commandLineArg[100];	
    char testCommandLineArg[100];
    char *documentation;
	// define endpoint patterns here
    void (*endpointLogic)(struct Endpoint* e,char *arg1, char *arg2);	
    void (*endpointUpdateLogic)(struct Endpoint* e,char *arg1, char *arg2, char *arg3);		
    void (*testEndpointLogic)(struct Endpoint* e,char *arg1, char *arg2);
	struct Endpoint* next; 
};

typedef struct Endpoint endpoint_t;



endpoint_t *createEndpoint(char *name, char *commandLineArg, char *docString){
	// create endpoint
	endpoint_t *newEndpoint = (endpoint_t*)malloc(sizeof(endpoint_t));	
	strcpy(newEndpoint->name, name); 
	strcpy(newEndpoint->commandLineArg, commandLineArg); 
	newEndpoint->documentation = (char*)malloc(strlen(docString) + 1); 
	strcpy(newEndpoint->documentation, docString); 

	char testCommand[256];
	strcpy(testCommand, "t-");  
	strcat(testCommand, newEndpoint->commandLineArg); 
	strcpy(newEndpoint->testCommandLineArg, testCommand);
	 
	return newEndpoint; 
}


void appendEndpoint(endpoint_t **head_ref, endpoint_t *e1){
	// add endpoint in linked list
	e1->next = (*head_ref); 
	(*head_ref) = e1; 	
}

void execEndpoints(char *argv[], endpoint_t *head_ref){
	while(head_ref != NULL){
		// execute endpoint functions		
		head_ref->endpointLogic(head_ref, argv[1], argv[2]); 
		head_ref->testEndpointLogic(head_ref, argv[1], argv[2]); 
		head_ref = head_ref->next; 
	}
}

void printEndpoints(endpoint_t *head_ref){
	// display endpoints
	char *column1 = "FLAG-COMMAND";     
    char *column2 = "TESTING"; 
    char *column3 = "DOCUMENTATION";
    // display 
    printf("\e[0;31m"); 
    printf("%c[1m",27);
    printf("%-17s  %-17s  %-30s\n", column1, column2, column3);
    printf("==========================================================\n");
	while(head_ref != NULL){
		printf("--%-15s  %-15s  %-25s\n", 
            head_ref->commandLineArg,
            head_ref->testCommandLineArg,
            head_ref->documentation
        );
		head_ref = head_ref->next; 
	}
}


void endpointFunction(struct Endpoint *e, char *myarg, char *myArg2){
	if(strcmp(myarg, e->commandLineArg) == 0){
		printf("Endpoint function \n"); 
	}	
}

// create the function you want to attach for endpoint
void testFunction(struct Endpoint* e, char *myarg, char *myArg2){
	// insert playlist to db
	if(strcmp(myarg, e->testCommandLineArg) == 0){
		printf("test endpoint \n"); 
	}
}



int main(int argc, char* argv[]){
	 
	endpoint_t *head = NULL;
	// create an endpoint, attach logic and test function	
	endpoint_t *e1 = createEndpoint(
						"create-playlist", 
						"cp",
						"function to do something"
					);
	e1->endpointLogic = endpointFunction; 
	e1->testEndpointLogic = testFunction; 	
	appendEndpoint(&head, e1);

	 
	endpoint_t *e2 = createEndpoint(
						"create-song", 
						"cs",
						"function to do something part 2"
					);
	e2->endpointLogic = endpointFunction; 
	e2->testEndpointLogic = testFunction; 	
	appendEndpoint(&head, e2); 

	
	if(strcmp(argv[1], "help") == 0){
		printEndpoints(head); 
	}

	execEndpoints(argv, head); 
	
}
