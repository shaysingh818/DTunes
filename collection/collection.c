#include "collection.h"


sqlite3* openDB(char *filename){
    sqlite3 *db;
    if(sqlite3_open(filename, &db)){
        printf("Could not open the db : \n ");

        printf("Could not open the db:  %s\n",sqlite3_errmsg(db));
        exit(-1);
    }
    return db;
}


// Get current time for date timestamp on create/update
char* getCurrentTime(){
    time_t rawtime;
    struct tm * timeinfo;
    // get time
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    // return current time
    return asctime(timeinfo);
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
	if(chdir(ADMS_PATH) != 0){
        perror("chdir() to /error failed");
        return FALSE;
    }
	mkdir(newCollection->name, S_IRWXU);


	return 1;  

	
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
    //generateBanner(100);
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




