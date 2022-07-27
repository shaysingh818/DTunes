#include "collection.h"
#include "../logging/log.h"

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

	strcpy(newCollection->name, name); 
	strcpy(newCollection->dateCreated, currentTime); 
	strcpy(newCollection->diskSpace, "0 GB"); 
	strcpy(newCollection->fileCount, "0 Files"); 

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
	sqlite3_step(sql); 
	sqlite3_close(db);


	// create folder with collection name
	char formattedName[100]; 
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

        // store values:
        strcpy(collections[indexCount]->name, column0);
        strcpy(collections[indexCount]->dateCreated, column1);
        strcpy(collections[indexCount]->diskSpace, column2);
        strcpy(collections[indexCount]->fileCount, column3);
        indexCount += 1;
    }


    sqlite3_close(db);
    return collections;

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
    generateBanner(100);
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
	char filepath[300]; 
	
	while((next_file = readdir(collectionsPath)) != NULL){
		sprintf(filepath, "%s/%s", ADMS_PATH, next_file->d_name); 
		remove(filepath); 
	}
	closedir(collectionsPath); 

    return TRUE;
}


int checkCollectionExist(char *collectionName){


	int status = FALSE; 
    // open db
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


int viewCollectionFiles(char *name){

	// check if collection exists
	int result = checkCollectionExist(name); 
	if(result == FALSE){
		dlog("ERROR", "COLLECTION DOES NOT EXIST"); 
		return FALSE; 
	}

	char collectionName[50]; 
	sprintf(collectionName, "collection-%s", name);
 
	char buffer[75]; 
	sprintf(buffer, "%s/%s", ADMS_PATH, collectionName);

		
    printf("\e[0;31m");

    DIR *audioFolder = opendir(buffer);
	struct dirent *entry;

    while((entry=readdir(audioFolder)) != NULL){
		
        int fileCondition1 = strcmp(entry->d_name, "..") == 0;
        int fileCondition2 = strcmp(entry->d_name, ".") == 0;
			
        if(fileCondition1 == 0 & fileCondition2 == 0){
			dlog("FILE", entry->d_name); 
		}
    }
	
	return TRUE; 
		 
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

	 
	dlog("CWD", buffer);

	DIR *folder;
    struct dirent *entry;
    int files = 0;
    folder = opendir(buffer);
    if(folder == NULL){
        perror("Unable to read directory\n");
    }


	while((entry=readdir(folder))){
        files++;
        // file information
        char *tempFileName = entry->d_name;
        char *currTime = getCurrentTime();

        // file name processing
        removeChar(tempFileName, ' ');
        removeChar(tempFileName, ',');
        removeChar(tempFileName, '\'');

        // get previous file name 
        char *idk = malloc(strlen(tempFileName) + 1);
        strcpy(idk, tempFileName);

		// ignore dot files
        int fileCondition1 = strcmp(entry->d_name, "..") == 0;
        int fileCondition2 = strcmp(entry->d_name, ".") == 0;
			
        if(fileCondition1 == 0 & fileCondition2 == 0){ 

			char newPath[1024]; 
			sprintf(newPath, "%s/%s", buffer, idk);

			
			char existingPath[1024]; 
			sprintf(existingPath, "%s/%s", buffer, entry->d_name);
		
			dlog("CURR FILE", existingPath); 
			dlog("NEW FILE", newPath);
			
			int result = renameFile(entry->d_name, idk); 
			if(result == FALSE){
				dlog("FAILED", "rename file"); 
			} 
		}


		/**

		USE THIS LATER FOR QUEUEING AUDIO FILES

        char *streamingPath = combineFileStrs(cwd, tempFileName);
		dlog("STREAMING PATH", streamingPath); 

        int fileCondition1 = strcmp(entry->d_name, "..") == 0;
        int fileCondition2 = strcmp(entry->d_name, ".") == 0;

        if(fileCondition1 == 0 & fileCondition2 == 0){
            //insertSong(entry->d_name, currTime, streamingPath);
            dlog("SYNC SONG", tempFileName);
        } */
    }

    closedir(folder);

    return TRUE;

}

