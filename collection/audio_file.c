#include "../endpoints/endpoints.h"
#include "audio_file.h"
#include "collection.h"
#include "../logging/log.h"

int createAudioFile(char *setName, char *setStreamingPath){

    // create data model for sqlite3 
    char *currentTime = getCurrentTime();
    audiofile_t *newAudioFile;
    newAudioFile = (audiofile_t*)malloc(sizeof(audiofile_t));

    strcpy(newAudioFile->name, setName);
    strcpy(newAudioFile->dateCreated, currentTime);
    strcpy(newAudioFile->streamingPath, setStreamingPath);

    // insert to sqlite3
    sqlite3 *db = openDB(DB_PATH);

    char *errMsg = 0;
    sqlite3_stmt *sql;

    // insert query
    int result = sqlite3_prepare_v2(
        db,
        INSERT_DB_FILE,
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

	sqlite3_bind_text(sql, 1, newAudioFile->name, -1, NULL);
    sqlite3_bind_text(sql, 2, newAudioFile->dateCreated, -1, NULL);
    sqlite3_bind_text(sql, 3, newAudioFile->streamingPath, -1, NULL);
    sqlite3_step(sql);
    sqlite3_close(db);


    return 1;
}



audiofile_t **initAudioFiles(int limit){

    audiofile_t **files = malloc(limit * sizeof(audiofile_t*));
    for(int i = 0; i < limit; i++){
        files[i] = malloc(sizeof(audiofile_t));
    }

    // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_DB_FILES;
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

        // store values:
        strcpy(files[indexCount]->name, column0);
        strcpy(files[indexCount]->dateCreated, column1);
        strcpy(files[indexCount]->streamingPath, column2);
        indexCount += 1;
    }

    sqlite3_close(db);
    return files;
}



int getAudioFileTableSize(){

    // get amount of playlists in db    
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = COUNT_DB_FILE;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query playlists:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    // load playlist limit
    int fileLimit;
    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){
        const char *columnValue = sqlite3_column_text(sql, 0);
        sscanf(columnValue, "%d", &fileLimit); // Using sscanf
    }

    // close db
    sqlite3_finalize(sql);
    sqlite3_close(db);
    return fileLimit;
}


int viewAudioFiles(){
    // get amount of playlists in db    
    // Call the load songs with limit
    int fileLimit = getAudioFileTableSize();
    audiofile_t **files = initAudioFiles(fileLimit);
    audiofile_t ***p = &files;

    // print header 
    printf("\n");
    printf("\e[0;31m");
    printf("%-45s %-25s\n", "Name", "Date");
    generateBanner(100);
    // View song in format for terminal
    for(int i = 0; i < fileLimit; i++){
        printf("%-45s %-25s", (*p)[i]->name, (*p)[i]->dateCreated);
    }
    printf("\n");

    // return true or false if view was successful
    return TRUE;
}


int checkAudioFileExist(char *name){

    // open db
    int status = FALSE;
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *res;
    char *sql = VIEW_DB_FILE;
    char *err_msg = 0;

    // print headers
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if(rc == SQLITE_OK){
        sqlite3_bind_text(res, 1, name, -1, NULL);
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

