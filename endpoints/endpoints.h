#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>


struct Endpoint {
    char name[100];
    char commandLineArg[100];
    char *documentation;
    void (*endpointLogic)(struct Endpoint* e, char* argv[]);
    struct Endpoint* next;
};

typedef struct Endpoint endpoint_t;


// endpoint library functions
endpoint_t *createEndpoint(char *name, char *commandLineArg, char *testCommand);
void appendEndpoint(endpoint_t **head_ref, endpoint_t *e1); 
void execEndpoints(int argc, char *argv[], endpoint_t *head_ref);
void printEndpoints(endpoint_t *head_ref);


 #endif 
