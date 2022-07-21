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

	// create uuid
	char collection_uuid[37]; 
	uuid_generate_time_safe(newCollection->collectionID); 
	uuid_unparse_lower(newCollection->collectionID, collection_uuid); 

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

	sqlite3_bind_text(sql, 1, newCollection->collectionID, -1, NULL);
	sqlite3_bind_text(sql, 2, newCollection->name, -1, NULL);
	sqlite3_bind_text(sql, 3, newCollection->dateCreated, -1, NULL);	
	sqlite3_bind_text(sql, 4, newCollection->diskSpace, -1, NULL);
	sqlite3_bind_text(sql, 5, newCollection->fileCount, -1, NULL);
	sqlite3_step(sql); 
	sqlite3_close(db);

	return 1;  

	// create folder with collection name
}

collection_t **initCollections(int limit){

    collection_t **collections = malloc(limit * sizeof(collection_t*));
    for(int i = 0; i < limit; i++){
        collections[i] = malloc(sizeof(collection_t));
    }

    // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_DB_COLLECTION;
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
        const char *column3 = sqlite3_column_text(sql, 2);
        const char *column4 = sqlite3_column_text(sql, 2);

        uuid_parse(column0, collections[indexCount]->collectionID);
        // store values:
        strcpy(collections[indexCount]->name, column1);
        strcpy(collections[indexCount]->dateCreated, column2);
        strcpy(collections[indexCount]->diskSpace, column3);
        strcpy(collections[indexCount]->fileCount, column4);
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
    printf("%-45s %-25s %-15s\n", "UUID", "Name", "Date");
    //generateBanner(100);
    // View song in format for terminal
    for(int i = 0; i < collectionLimit; i++){
        char collection_uuid[37];
        uuid_unparse_lower((*p)[i]->collectionID, collection_uuid);
        printf("%-45s %-25s %-15s", collection_uuid, (*p)[i]->name, (*p)[i]->dateCreated);
    }
    printf("\n");

    // return true or false if view was successful
    return TRUE;
}



int deleteCollection(char *collectionUUID){

    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;

    // prepare statement
    int result = sqlite3_prepare_v2(db, DELETE_DB_COLLECTION_UUID, -1, &sql, NULL);
    sqlite3_bind_text(sql, 1, collectionUUID, -1, NULL);

    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    sqlite3_step(sql);
    sqlite3_close(db);

	// delete folder of songs

    return TRUE;
}


int deleteAllCollections(){

     // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = DELETE_DB_COLLECTION;
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

    return TRUE;
}




