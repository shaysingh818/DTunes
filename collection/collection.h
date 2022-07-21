#ifndef COLLECTION_H
#define COLLECTION_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sqlite3.h>
#include <time.h> 
#include <uuid/uuid.h>
#include <dirent.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define ADMS_PATH "../adms/collections"
#define DB_PATH "../adms/db/dtunes.db"

/* collection db queries */ 
#define INSERT_DB_COLLECTION  "INSERT INTO COLLECTION VALUES(?,?,?,?,?)"
#define VIEW_DB_COLLECTION "SELECT * FROM COLLECTION"
#define COUNT_DB_COLLECTION "SELECT COUNT(*) FROM COLLECTION"
#define DELETE_DB_COLLECTION_UUID  "DELETE FROM COLLECTION where collection_uuid=?" 
#define DELETE_DB_COLLECTION  "DELETE  FROM COLLECTION"


struct Collection {
	uuid_t collectionID; 
	char name[100]; 
	char dateCreated[30]; 
	char diskSpace[100]; 
	char fileCount[100];
};

typedef struct Collection collection_t; 

// database utilties
sqlite3* openDB(char *filename); 
char* getCurrentTime(); 

// crud operations
collection_t **initCollections(int limit);
collection_t *viewCollectionUUID(char **uuid);  
int createCollection(char *name);
int getCollectionTableSize();  
int viewCollections(); 
int deleteCollection(char *collectionUUID); 
int deleteAllCollections(); 
int viewCollectionFiles(); 

#endif

