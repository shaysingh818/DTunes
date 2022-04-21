#include "urls.h"
#include "db.h"

url_t **initUrls(int limit){
/**
    Function for returning structure instance of a song
*/
    url_t **urls = malloc(limit * sizeof(url_t*));
    // allocate space
    for(int i = 0; i < limit; i++){
        urls[i] = malloc(sizeof(url_t));
    }

    // open db
    sqlite3 *db = openDB(DB_PATH);
    // prepare statement    
    sqlite3_stmt *sql;
    char *query = VIEW_DB_URLS;
    // prepare statement
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    // check for errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to view youtube urls:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    int indexCount = 0;
    // allocate results into struct array
    while ((result = sqlite3_step(sql)) == SQLITE_ROW) {
        // extract column values
        const char *column1 = sqlite3_column_text(sql, 0);
        const char *column2 = sqlite3_column_text(sql, 1);
        // store values
        strcpy(urls[indexCount]->url, column1);
        strcpy(urls[indexCount]->dateCreated, column2);
        indexCount += 1;
    }


    sqlite3_close(db);
    return urls;
}


int getUrlTableSize(){
    // get amount of songs in db    
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = COUNT_DB_URLS;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete song:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    // load song limit
    int urlLimit;
    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){
        const char *columnValue = sqlite3_column_text(sql, 0);
        sscanf(columnValue, "%d", &urlLimit); // Using sscanf
    }

    sqlite3_finalize(sql);
    sqlite3_close(db);

    return urlLimit;
}



int insertUrl(char *url, char *currentTime){
    url_t *newUrl;
    newUrl = (url_t*)malloc(sizeof(url_t));
    printf("\033[0;32m");
    d_log_time("STRUCTURE ALLOCATION", "CREATED YOUTUBE URL");

    // set values
    strcpy(newUrl->url, url);
    strcpy(newUrl->dateCreated, currentTime);

    int dbResult = createYoutubeUrl(newUrl);
    if(dbResult){
        if(DEBUG == TRUE){
            d_log_time("DB INSERT", "INSERT YOUTUBE URL");
            return TRUE;
        }
    }else{
        d_log_time("FAILED", "INSERT YOUTUBE URL FAILED");
        return FALSE;
    }
}



int createYoutubeUrl(url_t* newUrl){
    // open db
    sqlite3 *db = openDB(DB_PATH);
    // prepare statement    
    char *errMsg = 0;
    sqlite3_stmt *sql;
    char *query = INSERT_DB_URL;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);

    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to insert:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    // bind song variables to sqlite3 statment
    sqlite3_bind_text(sql, 1, newUrl->url, -1, NULL);
    sqlite3_bind_text(sql, 2, newUrl->dateCreated, -1, NULL);

    // do first insert
    sqlite3_step(sql);
    sqlite3_close(db);

    return 1;
}




int viewUrls(){

    // Call the load songs with limit   
    int urlLimit = getUrlTableSize();
    url_t **urls = initUrls(urlLimit);
    url_t ***p = &urls;

    // print header 
    printf("\n");
    printf("Url                                     Date\n");
    printf("================================================================================\n");
    // View song in format for terminal
    for(int i = 0; i < urlLimit; i++){
        printf("%-55s %s\n", (*p)[i]->url, (*p)[i]->dateCreated);
    }
    printf("\n");

    // return true or false if view was successful
    return TRUE;
}



int deleteYoutubeUrl(char *url){
    // open db
    sqlite3 *db = openDB(DB_PATH);
    // prepare statement
    sqlite3_stmt *sql;
    char *query = DELETE_DB_URL;
    // prepare statement
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    // bind vars to statement
    sqlite3_bind_text(sql, 1, url, -1, NULL);

    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete youtube url:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }


    printf("[DB OPERATION] Deleted youtube url\n");
    sqlite3_step(sql);
    sqlite3_close(db);

    return TRUE;
}




int deleteAllYoutubeUrls(){
    // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;

    char *query = DELETE_DB_URLS;
    char *errMsg = 0;

    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete urls:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

    printf("[DB OPERATION]: DELETE ALL YOUTUBE URLS\n");
    sqlite3_step(sql);
    sqlite3_close(db);

    return TRUE;
}



int youtubeDownloadBackup(){
    // Call the load songs with limit   
    int urlLimit = getUrlTableSize();
    url_t **urls = initUrls(urlLimit);
    url_t ***p = &urls;

	// change to desired path
	if(chdir(YOUTUBE_FILE_PATH) != 0){
        dlog("ERROR", "CHANGE DIR YOUTUBE");
        perror("chdir() to /error failed");
        return FALSE;
    }
	
    // View song in format for terminal
    for(int i = 0; i < urlLimit; i++){
        //downloadVideo((*p)[i]->url);
        backupVideo((*p)[i]->url);
    }

    return TRUE;
}


url_t **allocateSubArray(int threadCount){
    url_t **urlSubArr = malloc(threadCount * sizeof(url_t*));
    for(int j = 0; j < threadCount; j++){
        urlSubArr[j] = malloc(sizeof(url_t));
    }
    return urlSubArr;
}



void printSubArray(url_t **subArr, int arrSize){
    url_t ***p = &subArr;
    printf("[[");
    for(int i = 0; i < arrSize; i++){
        printf("%s ",(*p)[i]->url);
    }
    printf("]]\n");
}



void downloadUrls(url_t** ytUrls, int urlSize, int threadCount){

    pthread_t urlThreads[threadCount];
    url_t ***p = &ytUrls;

    int start, split, end, indexCount;
    for(int i = 0; i < urlSize; i+=threadCount){
        start = i;
        split = i + threadCount;
        end = urlSize - 1;
        // create sub array
        url_t **urlSubArr = allocateSubArray(threadCount);
        // populate the sub array with start, split indexes
        indexCount = 0;
        for(int j = start; j < split; j++){
            urlSubArr[indexCount] = (*p)[j];
            indexCount += 1;
        }

        printSubArray(urlSubArr, threadCount);

        if(end-split < threadCount){
            indexCount = 0;
            url_t **urlSubArr = allocateSubArray(threadCount);
            for(int j = split; j <= end; j++){
                urlSubArr[indexCount] = (*p)[j];
                indexCount += 1;
            }
            printSubArray(urlSubArr, (end-split)+1);
            break;
        }

        if(split == end){
            break;
        }

    }
}


