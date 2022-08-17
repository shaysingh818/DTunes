#include "endpoints.h"
#include <stdio.h> 



page_t *collectionsPage(){

	/**
		Example of creating endpoints for a module
	*/

	// create endpoint	
	endpoint_t *head = NULL; 
	
	char *args[][2] = {
		{"create", "create a collection, store in database"},
		{"name", "Name of the collection you want to create"},	
	}; 

	endpoint_t *create = constructEndpoint(
		"create-collection",
		"endpoint to create collections and store in databse",
		0, 2,	
		args
	);
	appendEndpoint(&head, create); 
 
	char *args1[][2] = {
		{"update", "update collection in the database"},
		{"name", "Name of the collection you want to update"},	
	}; 

	endpoint_t *update = constructEndpoint(
		"update-collection",
		"endpoint to update collections in the databse",
		0, 2,	
		args1
	); 
	appendEndpoint(&head, update); 

	// create page
	page_t *p = createPage(
		"collections",	
		"Endpoints for the collections module",
		head
	);

	return p; 
}


int main(int argc, char **argv){

	page_t *p = collectionsPage(); 
	printPageEndpoints(p); 
	//printPage(p); 		
	
}
