#include "endpoints.h"


endpoint_t *createEndpoint(char *name, char *documentation, int protocol, int numArgs){

	endpoint_t *e = (endpoint_t*)malloc(sizeof(endpoint_t));
	
	// allocate space for name and documentation
	size_t nameLength = strlen(name) + 1; 
	size_t documentationLength = strlen(documentation) + 1; 
	e->name = (char*)malloc(nameLength * sizeof(char)); 
	e->documentation = (char*)malloc(documentationLength * sizeof(char));

	// allocate number of arguments
	e->args = malloc(numArgs * sizeof(arg_t*)); 
	for(int i = 0; i < numArgs; i++){
		e->args[i] = malloc(sizeof(arg_t)); 
	} 

	// assign endpoint properties
	strcpy(e->name, name); 
	strcpy(e->documentation, documentation);
	e->numArgs = numArgs;  
	e->protocol = protocol; 

	return e; 
}


arg_t *createArgument(char *name, char *documentation){

	arg_t *a = (arg_t*)malloc(sizeof(arg_t));

	// allocate space for name and documentation	
	size_t nameLength = strlen(name) + 1; 
	size_t documentationLength = strlen(documentation) + 1; 
	a->name = (char*)malloc(nameLength * sizeof(char)); 
	a->documentation = (char*)malloc(documentationLength * sizeof(char)); 

	// assign name and documentation 
	strcpy(a->name, name); 
	strcpy(a->documentation, documentation); 

	return a; 
}


page_t *createPage(char *name, char *documentation, endpoint_t *head){

	page_t *p = (page_t*)malloc(sizeof(page_t));

	// allocate space for name and documentation	
	size_t nameLength = strlen(name) + 1; 
	size_t documentationLength = strlen(documentation) + 1; 
	p->name = (char*)malloc(nameLength * sizeof(char)); 
	p->documentation = (char*)malloc(documentationLength * sizeof(char));

	// assign name and documentation 
	strcpy(p->name, name); 
	strcpy(p->documentation, documentation);
	p->endpointCount = 0;  
	p->head = head; 

	return p; 
	
}


void appendEndpoint(endpoint_t **head_ref, endpoint_t *e){
	e->next = (*head_ref); 
	(*head_ref) = e; 
}



void generateBanner(int amount){
    char *str = malloc(amount * sizeof(char *));
    for(int i = 0; i < amount; i++){
        strcat(str, "=");
    }
    printf("%s\n", str);
    free(str);
}


void printPage(page_t *p){
	printf("\n"); 
    printf("\e[0;31m");
	printf(COLOR_BOLD "Module Name: %-15s\n", p->name);
	printf(COLOR_BOLD "Description: %-15s\n", p->documentation);  	
	printf(COLOR_BOLD "%s %d\n\n" COLOR_OFF, "Endpoint Count", p->endpointCount);
	printf("Type '%s help' to view the endpoints for this page\n", p->name); 
	printf("\n");  
}


void printEndpoint(endpoint_t *e){

	printf("\n"); 
    printf("\e[0;31m");

	printf(COLOR_BOLD "%-25s %-25s\n\n" COLOR_OFF, e->name, e->documentation); 	
	printf(COLOR_BOLD "Required arguments: \n" COLOR_OFF); 
	generateBanner(60); 	
	for(int i = 0; i < e->numArgs; i++){
		arg_t *currentArg = e->args[i];
		printf("%s : %s\n", currentArg->name, currentArg->documentation);  
	}

	printf("\n");
}
