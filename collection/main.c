#include "collection_endpoints.h"
#include <stdlib.h> 
#include <stdio.h> 


void generateBanner(int amount){
	char *str = malloc(amount * sizeof(char *));  
    for(int i = 0; i < amount; i++){
		strcat(str, "=");  
    }
    printf("%s\n", str);
	free(str); 
}

int main(int argc, char* argv[]){

    endpoint_t *head = NULL;

    collectionEndpoints(&head);

    if(strcmp(argv[1], "help") == 0){
        printEndpoints(head);
    }

    execEndpoints(argc, argv, head);
    printf("\n");

}

