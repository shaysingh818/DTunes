#include "endpoints.h"


endpoint_t *createEndpoint(char *name, char *documentation, int numArgs){

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


void constructEndpoint(
		char *name, char *documentation, 
		int argsLength,
		char *args[][2],
		void (*function)(struct Endpoint* e, char* argv[]),
		endpoint_t **head){

	
    endpoint_t *test = createEndpoint(
        name,
        documentation,
        argsLength
    );

    for(int i = 0; i < test->numArgs; i++){

        // iterate through args and create
        char *name = args[i][0];
        char *documentation = args[i][1];

        // create argument
        arg_t *a = createArgument(name, documentation);
        test->args[i] = a; 
    }

	test->endpointLogic = function; 
	appendEndpoint(head, test); 

}


page_t *searchPage(page_t *headRef, char *name){
	page_t *result = NULL; 
	while(headRef != NULL){
		if(strcmp(name, headRef->name) == 0){
			result = headRef; 
			return result; 
		}
		headRef = headRef->next; 
	}
	return result; 
}


endpoint_t *searchEndpoint(page_t *p, char *name){
	endpoint_t *result = NULL; 
	while(p->head != NULL){
		if(strcmp(name, p->head->name) == 0){
			result = p->head; 
			return result; 
		}
		p->head = p->head->next; 
	}
	return result; 
}

void appendEndpoint(endpoint_t **head_ref, endpoint_t *e){
	e->next = (*head_ref); 
	(*head_ref) = e; 
}


void appendPage(page_t **head_ref, page_t *p){
	p->next = (*head_ref); 
	(*head_ref) = p; 
}



void generateBanner(int amount){
    char *str = malloc(amount * sizeof(char *));
    for(int i = 0; i < amount; i++){
        strcat(str, "=");
    }
    printf("%s\n", str);
    free(str);
}


void printPages(page_t *headRef){
	printf("\n"); 
    printf("\e[0;31m");
	generateBanner(60); 
	while(headRef != NULL){
		printf(
			COLOR_BOLD "%-25s %-25s\n", 
			headRef->name, headRef->documentation
		); 	
		headRef = headRef->next; 
	}
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


void printPageEndpoints(page_t *p){

	printf("\n"); 
    printf("\e[0;31m");
	printf(COLOR_BOLD "Module: %s\n" COLOR_OFF,p->name); 	
	generateBanner(60); 	

	while(p->head != NULL){
		endpoint_t *currEndpoint = p->head;
		printf(
			COLOR_BOLD "%-25s %-25s\n" COLOR_OFF, 
			currEndpoint->name, currEndpoint->documentation
		); 	
		p->head = p->head->next; 
	}
	printf("\n"); 
}


void pageHelp(page_t *headRef, char *name){
	while(headRef != NULL){
		if(strcmp(name, headRef->name) == 0){
			printPageEndpoints(headRef); 
			break; 
		}
		headRef = headRef->next; 
	}
}


void endpointHelp(endpoint_t *headRef, char *name){
	while(headRef != NULL){
		if(strcmp(name, headRef->name) == 0){
			printEndpoint(headRef); 
			break; 
		}
		headRef = headRef->next; 
	}
}


void executePageEndpoints(page_t *p, char *argv[]){
	// iterate through page endpoints
	while(p->head != NULL){

		// execute with correct args supplied
		if(strcmp(argv[2], p->head->name) == 0){
			p->head->endpointLogic(p->head, argv); 
		}
		p->head = p->head->next; 
	}
}

void executionCycle(page_t *headRef, int argc, char *argv[]){

	for(int i = 0; i < argc; i++){
		char *currentArg = argv[i];
		switch(i){
			case 1:
				if(strcmp(currentArg, "help") == 0){
					printPages(headRef); 
				} 
				break;

			case 2:
				if(strcmp(currentArg, "help") == 0){
					pageHelp(headRef, argv[i-1]); 	
				} else {	
					// search execute endpoints for page
					page_t *p = searchPage(headRef, argv[i-1]); 
					executePageEndpoints(p, argv); 			
				}
				break; 

			case 3:
				if(strcmp(currentArg, "help") == 0){
					endpoint_t *e = searchEndpoint(headRef, argv[i-1]); 
					printEndpoint(e); 
				}
		} 
 
	}
}
