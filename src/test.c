#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>
#include <sqlite3.h>
#include "playlist.h"
#include "db.h"

#define TRUE 1
#define FALSE 0

#define DEBUG FALSE

struct Endpoint {
	char name[100];
	char commandLineArg[100]; 
	char *documentation; 
	void (*endpointLogic)(struct Endpoint* e,char *arg1, char *arg2); 	
	void (*testEndpointLogic)(const struct Endpoint* e,char *arg1, char *arg2); 
};

typedef struct Endpoint endpoint_t;


// function create an endpoint
endpoint_t *createEndpoint(char *name, char *commandLineArg){
	endpoint_t *e1;
    e1 = (endpoint_t*)malloc(sizeof(endpoint_t));
	// bind values
	strcpy(e1->name, name);
    strcpy(e1->commandLineArg, commandLineArg);
	return e1; 	
} 


void setDocumentationString(struct Endpoint* e, char *docBuffer){
	e->documentation = (char*)malloc(strlen(docBuffer) + 1);
	strcpy(e->documentation, "testing");
	printf("Doc buffer set\n"); 
}


// create the function you want to attach for endpoint
void testFunction(struct Endpoint* e, char *myarg, char *myArg2){
	// insert playlist to db
	if(strcmp(myarg, e->commandLineArg) == 0){
		printf("This is working %s\n", myarg); 
		printf("This is working %s\n", myarg); 
	}
}


// create the function you want to attach for endpoint
void createSong(struct Endpoint* e, char *myarg, char *myArg2){
	// insert playlist to db
	if(strcmp(myarg, e->commandLineArg) == 0){
		printf("Create song %s\n", myArg2); 
	}
}


// add endpoints to array
// display help menu for all

int main(int argc, char* argv[]){

	// create an endpoint, attach logic and test function	
	endpoint_t *e1 = createEndpoint("create-playlist", "cp");
	e1->endpointLogic = testFunction; // attach here
	setDocumentationString(e1, "create a playlist on dtunes"); 
	e1->endpointLogic(e1, argv[1], argv[2]);

	// create second endpoint 
	endpoint_t *e2 = createEndpoint("create-song", "cs");
	e2->endpointLogic = createSong; // attach here
	setDocumentationString(e2, "create a song on dtunes"); 
	e2->endpointLogic(e2, argv[1], argv[2]); 

}
