#include "../phaedra/phaedra.h"
#include "collection.h"
#include "../logging/log.h"
#include "../endpoints/endpoints.h"

sqlite3* openDB(char *filename){
    sqlite3 *db;
    if(sqlite3_open(filename, &db)){
        printf("Could not open the db : \n ");

        printf("Could not open the db:  %s\n",sqlite3_errmsg(db));
        exit(-1);
    }
    return db;
}


static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}



int createCollection(char *name){

	// create data model for sqlite3 
	char *currentTime = getCurrentTime(); 
	collection_t *newCollection; 
	newCollection = (collection_t*)malloc(sizeof(collection_t));
	
	/* get length of string params */ 
	size_t nameLength = strlen(name) + 1; 
	size_t dateLength = strlen(currentTime) + 1; 
	size_t spaceLength = strlen("0 GB") + 1; 
	size_t fileLength = strlen("0 Files") + 1;	
	size_t pathLength = strlen("/collection-") + strlen(ADMS_PATH) + strlen(name) + 1;

	newCollection->name = (char*)malloc(nameLength * sizeof(char));
	newCollection->dateCreated = (char*)malloc(dateLength * sizeof(char));
	newCollection->diskSpace = (char*)malloc(spaceLength * sizeof(char));	
	newCollection->fileCount = (char*)malloc(fileLength * sizeof(char));
	newCollection->collectionPath = (char*)malloc(pathLength * sizeof(char));
	
	strcpy(newCollection->name, name); 
	strcpy(newCollection->dateCreated, currentTime); 
	strcpy(newCollection->diskSpace, "0 GB"); 
	strcpy(newCollection->fileCount, "0 Files");
	
	/* format path */
	sprintf(
		newCollection->collectionPath,
		"%s/collection-%s", 
		ADMS_PATH, name
	); 		
	
	// insert to sqlite3
	sqlite3 *db = openDB(DB_PATH); 

	char *errMsg = 0; 
 	sqlite3_stmt *sql;

	// insert query
    int result = sqlite3_prepare_v2(
		db, 
		INSERT_DB_COLLECTION,
		-1, 
		&sql, 
		NULL
	);

	 // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to insert:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

	sqlite3_bind_text(sql, 1, newCollection->name, -1, NULL);
	sqlite3_bind_text(sql, 2, newCollection->dateCreated, -1, NULL);	
	sqlite3_bind_text(sql, 3, newCollection->diskSpace, -1, NULL);
	sqlite3_bind_text(sql, 4, newCollection->fileCount, -1, NULL);
	sqlite3_bind_text(sql, 5, newCollection->collectionPath, -1, NULL);
	sqlite3_step(sql); 
	sqlite3_close(db);

	size_t formatNameLength = strlen("collection-") + strlen(name) + 1;  	
	char *formattedName = (char*)malloc(formatNameLength * sizeof(char));
	sprintf(formattedName, "collection-%s", name); 

	if(chdir(ADMS_PATH) != 0){
        perror("chdir() to /error failed");
        return FALSE;
    }
	mkdir(formattedName, S_IRWXU);

	return 1;  

	
}


int updateCollectionById(char *uuid, char *newName){
    // open db
    char *sql;
    sqlite3 *db = openDB(DB_PATH);
    char *errMsg = 0;
    const char* data = "Callback function called";
    // prepare statement
    char string[200];
    sprintf(string, "UPDATE COLLECTION set name ='%s' WHERE collection_uuid='%s'; ", newName, uuid);
    sql = string; \
            "SELECT * from COLLECTION";

    int result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);

    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete song:  %s\n",errMsg);
         sqlite3_free(errMsg);
        return FALSE;
    }

    //sqlite3_step(sql); 
    sqlite3_close(db);

    return TRUE;
}


collection_t **initCollections(int limit){

    collection_t **collections = malloc(limit * sizeof(collection_t*));
    for(int i = 0; i < limit; i++){
        collections[i] = malloc(sizeof(collection_t));
    }

    // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_DB_COLLECTIONS;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to view playlists:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    // allocate results into struct array
    int indexCount = 0;
    while ((result = sqlite3_step(sql)) == SQLITE_ROW) {
        // extract column values    
        const char *column0 = sqlite3_column_text(sql, 0);
        const char *column1 = sqlite3_column_text(sql, 1);
        const char *column2 = sqlite3_column_text(sql, 2);
        const char *column3 = sqlite3_column_text(sql, 3);
        const char *column4 = sqlite3_column_text(sql, 4);

		size_t nameLength = strlen(column0) + 1; 
		size_t dateLength = strlen(column1) + 1; 
		size_t spaceLength = strlen(column2) + 1; 
		size_t fileLength = strlen(column3) + 1;	
		size_t pathLength = strlen(column4) + 1;

		collections[indexCount]->name = (char*)malloc(nameLength * sizeof(char));
		collections[indexCount]->dateCreated = (char*)malloc(dateLength * sizeof(char));
		collections[indexCount]->diskSpace = (char*)malloc(spaceLength * sizeof(char));	
		collections[indexCount]->fileCount = (char*)malloc(fileLength * sizeof(char));
		collections[indexCount]->collectionPath = (char*)malloc(pathLength * sizeof(char));
	
        // store values:
        strcpy(collections[indexCount]->name, column0);
        strcpy(collections[indexCount]->dateCreated, column1);
        strcpy(collections[indexCount]->diskSpace, column2);
        strcpy(collections[indexCount]->fileCount, column3);	
        strcpy(collections[indexCount]->collectionPath, column4);

        indexCount += 1;
    }

    sqlite3_close(db);
    return collections;
}


collection_t *viewCollection(char *collectionName){

	collection_t *collection; 
	collection = (collection_t*)malloc(sizeof(collection_t)); 
	
	sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_DB_COLLECTION;

	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query song by uuid:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

	sqlite3_bind_text(sql, 1, collectionName, -1, NULL);
    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){

		const char *column0 = sqlite3_column_text(sql, 0);
        const char *column1 = sqlite3_column_text(sql, 1);
        const char *column2 = sqlite3_column_text(sql, 2);
        const char *column3 = sqlite3_column_text(sql, 3);	
        const char *column4 = sqlite3_column_text(sql, 4);
	
		size_t nameLength = strlen(column0) + 1; 
		size_t dateLength = strlen(column1) + 1; 
		size_t spaceLength = strlen(column2) + 1; 
		size_t fileLength = strlen(column3) + 1;	
		size_t pathLength = strlen(column4) + 1;

		collection->name = (char*)malloc(nameLength * sizeof(char));
		collection->dateCreated = (char*)malloc(dateLength * sizeof(char));
		collection->diskSpace = (char*)malloc(spaceLength * sizeof(char));	
		collection->fileCount = (char*)malloc(fileLength * sizeof(char));
		collection->collectionPath = (char*)malloc(pathLength * sizeof(char));

		// copy fields
		strcpy(collection->name, column0); 
		strcpy(collection->dateCreated, column1); 
		strcpy(collection->diskSpace, column2); 
		strcpy(collection->fileCount, column3); 
		strcpy(collection->collectionPath, column4); 

	}

	sqlite3_finalize(sql);
    sqlite3_close(db);


	return collection; 
	
}


int getCollectionTableSize(){

    // get amount of playlists in db    
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = COUNT_DB_COLLECTION;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query playlists:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    // load playlist limit
    int collectionLimit;
    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){
        const char *columnValue = sqlite3_column_text(sql, 0);
        sscanf(columnValue, "%d", &collectionLimit); // Using sscanf
    }

    // close db
    sqlite3_finalize(sql);
    sqlite3_close(db);
    return collectionLimit;
}




int viewCollections(){
    // get amount of playlists in db    
    // Call the load songs with limit
    int collectionLimit = getCollectionTableSize();
    collection_t **collections = initCollections(collectionLimit);
    collection_t ***p = &collections;

    // print header 
    printf("\n");
    printf("\e[0;31m");
    printf("%-45s %-25s\n", "Name", "Date");
    generateBanner(75);
    // View song in format for terminal
    for(int i = 0; i < collectionLimit; i++){
        printf("%-45s %-25s", (*p)[i]->name, (*p)[i]->dateCreated);
    }
    printf("\n");

    // return true or false if view was successful
    return TRUE;
}



int deleteCollection(char *name){

    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;

    // prepare statement
    int result = sqlite3_prepare_v2(db, DELETE_DB_COLLECTION, -1, &sql, NULL);
    sqlite3_bind_text(sql, 1, name, -1, NULL);

    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    sqlite3_step(sql);
    sqlite3_close(db);

	// delete folder of songs
	if(chdir(ADMS_PATH) != 0){
        perror("chdir() to /error failed");
        return FALSE;
    }

	int ret = rmdir(name);
	if(ret == 0){
		printf("File directory removed\n"); 		
	}

    return TRUE;
}



int deleteAllCollections(){

     // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = DELETE_DB_COLLECTIONS;
    char *errMsg = 0;

    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete all collcetions:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    sqlite3_step(sql);
    sqlite3_close(db);

	// delete all folders
	DIR *collectionsPath = opendir(ADMS_PATH); 
	struct dirent *next_file; 
	char filepath[1000]; 
	
	while((next_file = readdir(collectionsPath)) != NULL){
		sprintf(filepath, "%s/%s", ADMS_PATH, next_file->d_name); 
		remove(filepath); 
	}
	closedir(collectionsPath); 

    return TRUE;
}


int deleteAllCollectionFileRelations(){

     // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = DELETE_DB_COLLECTION_FILES;
    char *errMsg = 0;

    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete all collcetions:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    sqlite3_step(sql);
    sqlite3_close(db);


    return TRUE;
}




int getRelationTableSize(char *collectionName){

	// get amount of files in collection
	sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = COUNT_COLLECTION_RELATIONS;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query playlist realation size:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
	
	sqlite3_bind_text(sql, 1, collectionName, -1, NULL); 
	int relationLimit;
    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){
        const char *columnValue = sqlite3_column_text(sql, 0);
        sscanf(columnValue, "%d", &relationLimit); 
    }

	sqlite3_finalize(sql);
    sqlite3_close(db);

	return relationLimit; 
}


int checkCollectionExist(char *collectionName){
	
    // open db
	int status = FALSE; 
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *res;
    char *sql = VIEW_DB_COLLECTION;
    char *err_msg = 0;

    // print headers
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if(rc == SQLITE_OK){
        sqlite3_bind_text(res, 1, collectionName, -1, NULL);
    }else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
	
    rc = sqlite3_step(res);
	if(rc == SQLITE_ROW){
		
		status = TRUE; 
	}

	sqlite3_finalize(res); 
	sqlite3_close(db);

	return status;  
}


int addFileToCollection(char *collectionName, char *fileName){
	
	sqlite3 *db = openDB(DB_PATH); 
	
	char *errMsg = 0; 
	sqlite3_stmt *sql; 
	int result = sqlite3_prepare_v2(db, ADD_COLLECTION_FILE, -1, &sql, NULL); 
	
	if(result != SQLITE_OK){
        fprintf(stderr, "Failed to insert:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

	uuid_t relation_uuid_str; 
	char relation_uuid[37]; 
	uuid_generate_time_safe(relation_uuid_str); 
	uuid_unparse_lower(relation_uuid_str, relation_uuid); 

	sqlite3_bind_text(sql, 1, relation_uuid, -1, NULL);	
    sqlite3_bind_text(sql, 2, collectionName, -1, NULL);
    sqlite3_bind_text(sql, 3, fileName, -1, NULL);

	sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
	
}


int syncCollectionsFilesToDB(char *collectionName){
	
	// check if collection exists
	int result = checkCollectionExist(collectionName); 
	if(result == FALSE){
		dlog("ERROR", "COLLECTION DOES NOT EXIST"); 
		return FALSE; 
	}

	collection_t *collection = viewCollection(collectionName); 
	char *path = collection->collectionPath; 

    DIR *audioFolder = opendir(path);
	struct dirent *entry;

    while((entry=readdir(audioFolder)) != NULL){
		
        int fileCondition1 = strcmp(entry->d_name, "..") == 0;
        int fileCondition2 = strcmp(entry->d_name, ".") == 0;
			
        if(fileCondition1 == 0 & fileCondition2 == 0){

			char buffer[1000]; 
			sprintf(buffer, "%s/%s", path, entry->d_name);
	
			int result = createAudioFile(
				entry->d_name, 
				buffer
			); 

			if(result != TRUE){
				return FALSE; 
			}

			int addResult = addFileToCollection(
				collectionName, 
				entry->d_name
			); 

			if(addResult != TRUE){
				dlog("FAILED TO SYNC", buffer); 
			} 
		}
    }

	return TRUE; 	
}



int syncAllCollectionsFilesToDB(){


	// delete all collection file relationships
	int relationDelete = deleteAllCollectionFileRelations(); 
	if(relationDelete != TRUE){
		dlog("SYNC FAILED", "Could not delete relations"); 
		return FALSE; 
	}	

	// load all collections
    int collectionLimit = getCollectionTableSize();
 	collection_t **collections = initCollections(collectionLimit);
    collection_t ***p = &collections;

    // print header 
    printf("\n");
    printf("\e[0;31m");
	
    // Clean their files names	
    for(int i = 0; i < collectionLimit; i++){
		renameCollectionFiles((*p)[i]->name);
    }

	// sync them	 
	int counter = 0; 
    for(int i = 0; i < collectionLimit; i++){
		int result = syncCollectionsFilesToDB((*p)[i]->name); 
		if(result == FALSE){
			return FALSE; 
		}
		counter++; 
    }

	printf("[MESSAGE]: Synced songs to %d collections\n", counter);
	return TRUE; 
		
}


audiofile_t **loadCollectionFiles(char *collectionName){

	int limit = getRelationTableSize(collectionName); 
	audiofile_t **files = malloc(limit * sizeof(audiofile_t*)); 
	for(int i = 0; i < limit; i++){
		files[i] = malloc(sizeof(audiofile_t)); 
	}

	sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql; 
    char *query = VIEW_DB_COLLECTION_FILES; 
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to view songs: %s\n", sqlite3_errmsg(db)); 
		sqlite3_close(db); 
	}

	sqlite3_bind_text(sql, 1, collectionName, -1, NULL); 
	int indexCount = 0; 
	while((result = sqlite3_step(sql)) == SQLITE_ROW){
		const char *fileName  = sqlite3_column_text(sql, 2);
		char name[1000];  
		strcpy(name, fileName);
		audiofile_t *myFile = viewAudioFile(name); 
		files[indexCount] = myFile;  
		indexCount += 1;
	}

	sqlite3_close(db); 
	return files; 
}


int viewCollectionFiles(char *name){

	int relationLimit = getRelationTableSize(name); 
	audiofile_t **files = loadCollectionFiles(name); 
	audiofile_t ***p = &files;
	collection_t *collection = viewCollection(name); 

	printf("\n"); 	
    printf("\e[0;31m");
	for(int i = 0; i < relationLimit; i++){
		dlog("FILE DB", files[i]->name); 
	}
	printf("\n");  
	
	return TRUE; 
}


void queueCollectionFiles(char *name){

	// check if collection exists
	int result = checkCollectionExist(name); 
	if(result == FALSE){
		dlog("ERROR", "COLLECTION DOES NOT EXIST"); 
	}

	// get collection folder name
	char collectionName[50]; 
	sprintf(collectionName, "collection-%s", name);

	// generate base streaming paths
	char baseStreamPath[256]; 	
	sprintf(baseStreamPath, "%s/%s", ADMS_PATH, collectionName);

	printf("\e[0;32m"); 
	dlog("BASE PATH", baseStreamPath);
	
	
    DIR * dirp;
    struct dirent * entry;
	
	// create string array of file names
	queue_t *q = initQueue(1000); 
	int arrayIndex = 0; 
	int fileCount = countFiles(baseStreamPath);
	char **fileNames = malloc(fileCount * sizeof(char*));
    for(int i = 0; i < fileCount; i++){
        fileNames[i] = malloc(512 * sizeof(char));
    }
 
    dirp = opendir(baseStreamPath); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {

		// ignore dot files
        int fileCondition1 = strcmp(entry->d_name, "..") == 0;
        int fileCondition2 = strcmp(entry->d_name, ".") == 0;
		
        if(fileCondition1 == 0 & fileCondition2 == 0){

			char streamingPath[512];	
			sprintf(streamingPath, "%s/%s", baseStreamPath, entry->d_name);
			strcpy(fileNames[arrayIndex], streamingPath);			
			
			arrayIndex += 1; 
		} 
	}


    closedir(dirp);
	
	// iterate through array of filename and add to queue
	for(int i = 0; i < fileCount; i++){
		enqueue(q, fileNames[i]);
		dlog("ENQUEUE", fileNames[i]);  
	}
	
    printf("\e[0;37m");
	dlog("FREE", "Freed file names");	
	free(fileNames);
	playQueue(q); 
}



char* combineFileStrs(const char *cwd, const char *fileName){
    char *result = malloc(strlen(cwd) + strlen(fileName) + 1);
    strcpy(result, cwd);
    strcat(result, fileName);
    return result;
}


void removeChar(char *testString, char charToRemove){
    char *d = testString;
    do {
        while (*d == charToRemove){
            ++d;
        }
    } while(*testString++ = *d++);
}



int renameFile(char *fileName, char *newFileName){
    int result = rename(fileName, newFileName);
    if(result == 0){
		return TRUE; 
    }
	return FALSE; 
}



// function to count amount of files in directory
int countFiles(char *directoryPath){

    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(directoryPath); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
         file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}


void clearAudioFileDirectory(char *desiredPath){
    DIR *audioFolder = opendir(desiredPath);
    struct dirent *next_file;
    char filepath[300];

    while((next_file=readdir(audioFolder)) != NULL){
        sprintf(filepath, "%s/%s", desiredPath, next_file->d_name);
        remove(filepath);
    }
    closedir(audioFolder);
}




int renameCollectionFiles(char *name){


	// check if collection exists
	int result = checkCollectionExist(name); 
	if(result == FALSE){
		dlog("ERROR", "COLLECTION DOES NOT EXIST"); 
		return FALSE; 
	}

	if(chdir(ADMS_PATH) != 0){
        perror("chdir() to /error failed");
        return FALSE;
    }

	// format collection name
	char collectionName[50]; 
	sprintf(collectionName, "collection-%s", name);

	
    char cwd[256];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        dlog("ERROR", "CURRENT WORKING DIRECTORY");
    }
    strcat(cwd, "/");

 
	char buffer[512]; 
	sprintf(buffer, "%s%s", cwd, collectionName);

	DIR *folder;
    struct dirent *entry;
    int files = 0;
    folder = opendir(buffer);
    if(folder == NULL){
        perror("Unable to read directory\n");
    }


	while((entry=readdir(folder))){

        files++;
	
		// create copy of current file name
		char *currFileName = malloc(strlen(entry->d_name) + 1); 
		strcpy(currFileName, entry->d_name); 

        // file information processing
        char *tempFileName = entry->d_name; 
        removeChar(tempFileName, ' ');
        removeChar(tempFileName, ',');
        removeChar(tempFileName, '\'');


		// ignore dot files
        int fileCondition1 = strcmp(entry->d_name, "..") == 0;
        int fileCondition2 = strcmp(entry->d_name, ".") == 0;
			
        if(fileCondition1 == 0 & fileCondition2 == 0){ 

			// format new path for processed file name
			char newPath[1024]; 
			sprintf(newPath, "%s/%s", buffer, tempFileName);

			// format new path for existing path
			char existingPath[1024]; 
			sprintf(existingPath, "%s/%s", buffer, currFileName);
			
			int result = renameFile(existingPath, newPath); 
			if(result == FALSE){
				dlog("FAILED", "rename file"); 
			} 
		}

    }

    closedir(folder);

	
	if(chdir("../../") != 0){
        perror("chdir() to /error failed");
        return FALSE;
    }

    return TRUE;

}

