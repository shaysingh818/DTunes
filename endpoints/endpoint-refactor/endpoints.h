#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"


struct Argument {
	char *name; 
	char *documentation; 
};

typedef struct Argument arg_t; 


struct Endpoint {
    char *name;
    char *documentation;
    arg_t **args;
	int numArgs;  
	int protocol; 
    void (*endpointLogic)(struct Endpoint* e, char* argv[]);
    struct Endpoint* next;
};

typedef struct Endpoint endpoint_t;


struct Page {
	char *name; 
	char *documentation; 
	int endpointCount; 
	endpoint_t *head;
	struct Page* next;  
}; 

typedef struct Page page_t;


endpoint_t *createEndpoint(char *name, char *documentation, int protocol, int numArgs); 
page_t *createPage(char *name, char *documentation, endpoint_t * head); 
arg_t *createArgument(char *name, char *documentation); 


// special methods
endpoint_t *constructEndpoint(
	char *name, char *documentation,
	int protocol, int argsLength, 
	char *args[][2]
); 

// mutators
void appendEndpoint(endpoint_t **head_ref, endpoint_t *e); 

// banners
void generateBanner(int amount);  

// printers
void printPage(page_t *p); 
void printEndpoint(endpoint_t *e); 
void printPageEndpoints(page_t *p); 
void describeEndpoint(endpoint_t *e, char *name); 


#endif
