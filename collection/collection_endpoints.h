#ifndef COLLECTION_ENDPOINTS_H
#define COLLECTION_ENDPOINTS_H

#include "../endpoints/endpoints.h"

// playlist functions
void insertCollectionCmd(endpoint_t *e, char* argv[]);
void viewCollectionsCmd(endpoint_t *e, char* argv[]);
void deleteCollectionCmd(endpoint_t *e, char* argv[]); 
void deleteAllCollectionsCmd(endpoint_t *e, char* argv[]);
void viewCollectionFilesCmd(endpoint_t *e, char* argv[]);
void syncCollectionFilesCmd(endpoint_t *e, char* argv[]); 
void queueCollectionFilesCmd(endpoint_t *e, char* argv[]); 
void cleanCollectionFilesCmd(endpoint_t *e, char* argv[]);
void runCollectionTestsCmd(endpoint_t *e, char* argv[]); 

// page 
page_t *collectionsModule(); 

#endif

