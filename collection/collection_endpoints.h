#ifndef COLLECTION_ENDPOINTS_H
#define COLLECTION_ENDPOINTS_H

#include "../endpoints/endpoints.h"

// playlist functions
void insertCollectionCmd(endpoint_t *e, char* argv[]);
void viewCollectionsCmd(endpoint_t *e, char* argv[]);
void deleteCollectionCmd(endpoint_t *e, char* argv[]); 
void deleteAllCollectionsCmd(endpoint_t *e, char* argv[]);
void collectionEndpoints(endpoint_t **head); 

#endif

