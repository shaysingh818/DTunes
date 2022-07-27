#include "collection.h"
#include "collection_endpoints.h"
#include "collection_test.h"
#include "../logging/log.h"


// create playlist
void insertCollectionCmd(endpoint_t *e, char* argv[]){
    if(strcmp(argv[1], e->commandLineArg) == 0){

        // insert collection to system
        int insertDbResult = createCollection(argv[2]);
        if(insertDbResult){
            printf("\033[0;32m");
            printf("[DTUNES]: Created Playlist\n");
        }else{
            printf("\e[0;31m");
            printf("Something went wrong, refer to test cases\n");
        }

    }
}


void viewCollectionsCmd(endpoint_t *e, char* argv[]){
	if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = viewCollections();
        printf("\n");
    }
}


void deleteCollectionCmd(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = deleteCollection(argv[2]);
        if(result){
            printf("[DTUNES]: Deleted Collection: %s\n", argv[2]);
        }else{
            printf("Something went wrong: refer to unit tests\n");
        }
    }
}


void deleteAllCollectionsCmd(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = deleteAllCollections();
        if(result){
            printf("[DTUNES]: Deleted Collections\n");
        }else{
            printf("Something went wrong: refer to unit tests\n");
        }
    }
}



void viewCollectionFilesCmd(endpoint_t *e, char* argv[]){

    if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = viewCollectionFiles(argv[2]);
        if(!result){
			dlog("FAILED", "VIEW COLLECTION FILES"); 
		}
    }

}



void cleanCollectionFilesCmd(endpoint_t *e, char* argv[]){	
	
    if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = renameCollectionFiles(argv[2]);
        if(!result){
			dlog("FAILED", "CLEAN COLLECTION FILES"); 
		}
    }
}


void runCollectionTestsCmd(endpoint_t *e, char* argv[]){	
    if (strcmp(argv[1], e->commandLineArg) == 0){
		runCollectionTests(); 
    }

}


void collectionEndpoints(endpoint_t **head) {

	// create collection
    endpoint_t *e1 = createEndpoint(
        "create-collection",
        "cc",
        "create audio collection in DTunes"
    );
    e1->endpointLogic = insertCollectionCmd;
    appendEndpoint(head, e1);


	// view collections
    endpoint_t *e2 = createEndpoint(
        "view-collection",
        "vc",
        "view audio collections on DTunes"
    );
    e2->endpointLogic = viewCollectionsCmd;
    appendEndpoint(head, e2);


	// delete collection by id
    endpoint_t *e3 = createEndpoint(
        "delete-collection",
        "dc",
        "view audio collection by uuid"
    );
    e3->endpointLogic = deleteCollectionCmd;
    appendEndpoint(head, e3);


	// delete all collections
    endpoint_t *e4 = createEndpoint(
        "delete-all-collection",
        "dac",
        "delete all collections"
    );
    e4->endpointLogic = deleteAllCollectionsCmd;
    appendEndpoint(head, e4);


	// test collections module
    endpoint_t *e5 = createEndpoint(
        "test-collections",
        "tc",
        "test collection methods"
    );
    e5->endpointLogic = runCollectionTestsCmd;
    appendEndpoint(head, e5);

	
	// view collectionf files
    endpoint_t *e6 = createEndpoint(
        "view-collection-files",
        "vcf",
        "view audio files in collection"
    );
    e6->endpointLogic = viewCollectionFilesCmd;
    appendEndpoint(head, e6);

	
	// clean collection files
    endpoint_t *e7 = createEndpoint(
        "clean-collection-files",
        "ccf",
        "clean audio file names in collection"
    );
    e7->endpointLogic = cleanCollectionFilesCmd;
    appendEndpoint(head, e7);

}
