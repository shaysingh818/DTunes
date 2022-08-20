#include "endpoints.h"
#include <stdio.h> 
#include <pthread.h> 

// endpoint defined functions
void createCollectionCmd(endpoint_t *e, char *argv[]){

	char  *name = argv[3]; 
	printf("Create collection %s\n", name); 

}


void updateCollectionCmd(endpoint_t *e, char *argv[]){
	
	char  *name = argv[3]; 
	printf("Update collection %s\n", name); 
}



void queuePhaedraCmd(endpoint_t *e, char *argv[]){
	
	char  *name = argv[3]; 
	printf("Queue songs %s\n", name); 
}



void playCmd(endpoint_t *e, char *argv[]){
	
	char  *name = argv[3]; 
	printf("Play audio file %s\n", name); 
}



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

	constructEndpoint(
		"create",
		"endpoint to create collections and store in databse",
		0, 2,	
		args,
		createCollectionCmd, 
		&head
	);

 
	char *args1[][2] = {
		{"update", "update collection in the database"},
		{"name", "Name of the collection you want to update"},	
	}; 

	constructEndpoint(
		"update",
		"endpoint to update collections in the database",
		0, 2,	
		args1,
		updateCollectionCmd, 
		&head
	); 


	// create page
	page_t *p = createPage(
		"collections",	
		"Module for storing audio collections in DTunes",
		head
	);

	return p; 
}


page_t *phaedraPage(){

	// create endpoint	
	endpoint_t *head = NULL; 
	
	char *args[][2] = {
		{"play", "play song"},
		{"name", "name of wav file"},	
	}; 

	constructEndpoint(
		"play",
		"endpoint to play file on phaedra",
		0, 2,	
		args,
		playCmd, 
		&head
	);

 
	char *args1[][2] = {
		{"queue", "queue files on phaedra"},
		{"name", "Name of the queue"},	
	}; 

	constructEndpoint(
		"queue",
		"queue files on phaedra",
		0, 2,	
		args1,
		queuePhaedraCmd, 
		&head
	);

	// create page
	page_t *p = createPage(
		"phaedra",	
		"Audio player module for DTunes",
		head
	);

	return p; 
}


int main(int argc, char **argv){


	page_t *head = NULL; 

	// create first page
	page_t *p = collectionsPage(); 
	appendPage(&head, p); 

	page_t *p1 = phaedraPage(); 
	appendPage(&head, p1); 

	//printPageEndpoints(p); 
	//printPages(head); 		
	executionCycle(head, argc, argv); 
	
}
