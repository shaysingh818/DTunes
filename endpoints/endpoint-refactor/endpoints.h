#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

#define BUFFER_LENGTH 1024
#define DEFAULT_PORT "8888"
#define ENDPOINT_IP "127.0.0.1"

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


// structural methods
endpoint_t *createEndpoint(char *name, char *documentation, int protocol, int numArgs); 
page_t *createPage(char *name, char *documentation, endpoint_t * head); 
arg_t *createArgument(char *name, char *documentation); 

// special methods
void constructEndpoint(
	char *name, char *documentation,
	int protocol, int argsLength,
	char *args[][2], 
	void (*function)(struct Endpoint* e, char* argv[]),
	endpoint_t **head
); 


// return search by name
page_t *searchPage(page_t *headRef, char *name); 
endpoint_t *searchEndpoint(page_t *p, char *name); 

// mutators
void appendEndpoint(endpoint_t **head_ref, endpoint_t *e); 
void appendPage(page_t **head_ref, page_t *p); 

// banners
void generateBanner(int amount);  

// printers
void printPages(page_t *p); 
void printEndpoint(endpoint_t *e); 
void printPageEndpoints(page_t *p); 
void pageHelp(page_t *headRef, char *name); 
void endpointHelp(endpoint_t *headRef, char *name); 

// execution cycles
void executionCycle(page_t *headRef, int argc, char *argv[]); 



#endif
