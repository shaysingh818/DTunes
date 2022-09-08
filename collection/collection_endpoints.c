#include "collection.h"
#include "collection_endpoints.h"
#include "collection_test.h"
#include "../logging/log.h"


// create playlist
void insertCollectionCmd(endpoint_t *e, char* argv[]){

    // insert collection to system
    int insertDbResult = createCollection(argv[3]);
    if(insertDbResult){
        printf("\033[0;32m");
        printf("[DTUNES]: Created Playlist\n");
    }else{
        printf("\e[0;31m");
        printf("Something went wrong, refer to test cases\n");
    }

}


void viewCollectionsCmd(endpoint_t *e, char* argv[]){

    int result = viewCollections();
    printf("\n");
    
}


void deleteCollectionCmd(endpoint_t *e, char* argv[]){

    // delete collection
    int result = deleteCollection(argv[3]);
    if(result){
        printf("[DTUNES]: Deleted Collection: %s\n", argv[2]);
    }else{
        printf("Something went wrong: refer to unit tests\n");
    }
}


void deleteAllCollectionsCmd(endpoint_t *e, char* argv[]){
    int result = deleteAllCollections();
    if(result){
        printf("[DTUNES]: Deleted Collections\n");
    }else{
        printf("Something went wrong: refer to unit tests\n");
    }
}



void viewCollectionFilesCmd(endpoint_t *e, char* argv[]){

    int result = viewCollectionFiles(argv[3]);
    if(!result){
        dlog("FAILED", "VIEW COLLECTION FILES"); 
    }

}



void cleanCollectionFilesCmd(endpoint_t *e, char* argv[]){	
	
    int result = renameCollectionFiles(argv[3]);
    if(!result){
        dlog("FAILED", "CLEAN COLLECTION FILES"); 
    }
}



void queueCollectionFilesCmd(endpoint_t *e, char* argv[]){	

    // queue files in collection
    queueCollectionFiles(argv[3]); 

}


void runCollectionTestsCmd(endpoint_t *e, char* argv[]){

    // test library
    runCollectionTests();
	printf("\n");  
	
}


page_t *collectionsModule(){
    
    endpoint_t *head = NULL;

	// create collection
    char *args[][2] = {
        {"create", "create a collection, store in database"},
        {"name", "Name of the collection you want to create"},
    };

    constructEndpoint(
        "create",
        "endpoint to create collections and store in databse",
        0, 2,
        args,
        insertCollectionCmd,
        &head
    );

	// view collections
    char *args1[][2] = {
        {"view", "view collections in database"}
    };

    constructEndpoint(
        "view",
        "view collections in database",
        0, 1,
        args1,
        viewCollectionsCmd,
        &head
    );

	// delete collection by id	
    char *args2[][2] = {
        {"delete", "delete collection"},
        {"id", "id of collection you want to delete"},
    };

    constructEndpoint(
        "delete",
        "endpoint to delete specific collection",
        0, 2,
        args2,
        deleteCollectionCmd,
        &head
    );

	// delete all collections	
    char *args3[][2] = {
        {"delete-all", "delete all collections"}
    };

    constructEndpoint(
        "delete-all",
        "endpoint to delete specific collection",
        0, 1,
        args3,
        deleteAllCollectionsCmd,
        &head
    );


	// view collection files
    char *args4[][2] = {
        {"vcf", "view collection files"},
        {"name", "name of collection"},
    };

    constructEndpoint(
        "vcf",
        "view files in a collection",
        0, 2,
        args4,
        viewCollectionFilesCmd,
        &head
    );


	// clean collection files
    char *args5[][2] = {
        {"ccf", "clean collection files"},
        {"name", "name of collection"},
    };

    constructEndpoint(
        "ccf",
        "format file names in a collection",
        0, 2,
        args5,
        cleanCollectionFilesCmd,
        &head
    );

	// queue collection files
    char *args6[][2] = {
        {"queue", "queue collection files"},
        {"name", "name of collection"},
    };

    constructEndpoint(
        "queue",
        "endpoint to queue files in a collection",
        0, 2,
        args6,
        queueCollectionFilesCmd,
        &head
    );


	// test collections
    char *args7[][2] = {
        {"test", "test collections module"}
    };

    constructEndpoint(
        "test",
        "test collections module",
        0, 1,
        args7,
        runCollectionTestsCmd,
        &head
    );


	// create page
    page_t *p = createPage(
        "collections",
        "Collections to store audio files in DTunes",
        head
    );

    return p;	

}

