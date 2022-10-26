#ifndef COLLECTION_H
#define COLLECTION_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sqlite3.h>
#include <sys/stat.h> 
#include <time.h> 
#include <uuid/uuid.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include "audio_file.h"


#define TRUE 1
#define FALSE 0
#define DEBUG TRUE

#define ADMS_PATH "adms/collections"
#define DB_PATH "adms/db/dtunes.db"

/* collection db queries */ 
#define INSERT_DB_COLLECTION  "INSERT INTO COLLECTION VALUES(?,?,?,?,?)"
#define VIEW_DB_COLLECTIONS "SELECT * FROM COLLECTION"
#define VIEW_DB_COLLECTION "SELECT * FROM COLLECTION WHERE name=?"
#define ADD_COLLECTION_FILE "INSERT INTO COLLECTION_FILE VALUES(?,?,?)"
#define VIEW_DB_COLLECTION_FILES  "SELECT * FROM COLLECTION_FILE WHERE collection=?"
#define COUNT_DB_COLLECTION "SELECT COUNT(*) FROM COLLECTION"
#define COUNT_COLLECTION_RELATIONS "SELECT COUNT(*) FROM COLLECTION_FILE WHERE collection=?"
#define DELETE_DB_COLLECTION  "DELETE FROM COLLECTION WHERE name=?" 
#define DELETE_DB_COLLECTIONS  "DELETE  FROM COLLECTION"
#define DELETE_DB_COLLECTION_FILES  "DELETE  FROM COLLECTION_FILE"


struct Collection {
	char *name; 
	char *dateCreated; 
	char *diskSpace; 
	char *fileCount;
	char *collectionPath; 
};

typedef struct Collection collection_t; 

// database utilties
sqlite3* openDB(char *filename); 
char* getCurrentTime(); 

// database operations
collection_t **initCollections(int limit);
collection_t *viewCollection(char *name);  

/* crud operations */ 
int createCollection(char *name);
int updateCollectionById(char *uuid, char *newName); 
int getCollectionTableSize();  
int viewCollections(); 
int deleteCollection(char *name); 
int deleteAllCollections();
int deleteAllCollectionFileRelations();
int checkCollectionExists(char *collectionName);

/* one to many relations */ 
int getRelationTableSize(char *collectionName);  
int addFileToCollection(char *collectionName, char *fileName); 
int viewCollectionFiles(char *name);

/* sync functions */ 
int syncCollectionsFilesToDB(char *collectionName);
int syncAllCollectionsFilesToDB(); 

/* queues */ 
void queueCollectionFiles(char *name);  

/* helpers */
char* combineFileStrs(const char *cwd, const char *fileName); 
void removeChar(char *testString, char charToRemove);
int renameFile(char *fileName, char *newFileName); 
int countFiles(char *directoryPath); 
void clearAudioFileDirectory(char *desiredPath);  
int renameCollectionFiles(char *collectionName); 

#endif
