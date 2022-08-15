#include "endpoints.h"
#include <stdio.h> 


int main(int argc, char **argv){

	// endpoint head
	endpoint_t *head = NULL; 

	// example creating an endpoint
	endpoint_t *test = createEndpoint(
		"collections", 
		"Endpoint for testing collections",
	 	0,2
	);

	arg_t *a1 = createArgument(
		"create", 
		"create a collection, store in database"
	); 

	
	arg_t *a2 = createArgument(
		"name", 
		"name of collection"
	);

	// attach args
	test->args[0] = a1; 
	test->args[1] = a2;
	appendEndpoint(&head, test);


	// example creating an endpoint
	endpoint_t *test2 = createEndpoint(
		"collections", 
		"Endpoint for testing collections",
	 	0,2
	);

	arg_t *a3 = createArgument(
		"update", 
		"update a collection, store in database"
	); 

	
	arg_t *a4 = createArgument(
		"name", 
		"name of collection"
	);

	// attach args
	test2->args[0] = a3; 
	test2->args[1] = a4;
	appendEndpoint(&head, test2);

	// create page
	page_t *p = createPage(
		"collections",	
		"Endpoints for the collections module",
		head
	); 

	printPage(p); 
		

	// print endpoint information
	//printEndpoint(test, "create-collection"); 
	
}
