#include "endpoints/endpoints.h"
#include "phaedra/phaedra_endpoints.h"
#include "collection/collection_endpoints.h"

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


void printDTunesBanner(FILE *fptr){
    char read_string[128];
    while(fgets(read_string,sizeof(read_string),fptr) != NULL){
        printf("%s",read_string);
	}
}


int main(int argc, char* argv[]){

	
	char *filename = "banner.txt"; 
	FILE *fptr = NULL; 

	if((fptr = fopen(filename, "r")) == NULL){
		printf("Banner file doesn't exist\n"); 
		return 1; 
	}
		
	printDTunesBanner(fptr);

	page_t *head = NULL; 
	
	page_t *p = phaedraModule();
	appendPage(&head, p); 
 
	page_t *p1 = collectionsModule();
	appendPage(&head, p1); 

	executionCycle(head, argc, argv); 
	
	return 0; 
}	
