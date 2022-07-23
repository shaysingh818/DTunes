#include "collection_endpoints.h"
#include <stdlib.h> 
#include <stdio.h> 



int main(int argc, char* argv[]){

    endpoint_t *head = NULL;

    collectionEndpoints(&head);

    if(strcmp(argv[1], "help") == 0){
        printEndpoints(head);
    }

    execEndpoints(argc, argv, head);
    printf("\n");

}

