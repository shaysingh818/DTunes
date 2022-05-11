#include "urls.h"
#include "../db/db.h"

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
        const char *column0 = sqlite3_column_text(sql, 0);
        const char *column1 = sqlite3_column_text(sql, 1);
        const char *column2 = sqlite3_column_text(sql, 2);
        // store values
		uuid_parse(column0, urls[indexCount]->urlId);
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

	// generate uuid
	char url_uuid[37];
    uuid_generate_time_safe(newUrl->urlId);
    uuid_unparse_lower(newUrl->urlId, url_uuid);

    // bind song variables to sqlite3 statment	
    sqlite3_bind_text(sql, 1, url_uuid, -1, NULL);
    sqlite3_bind_text(sql, 2, newUrl->url, -1, NULL);
    sqlite3_bind_text(sql, 3, newUrl->dateCreated, -1, NULL);

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
 	printf("\e[0;31m");
    printf("%-45s %-25s\n", "UUID", "URL");
	generateBanner(80);
    // View song in format for terminal
    for(int i = 0; i < urlLimit; i++){
		char url_uuid[37]; 
		uuid_unparse_lower((*p)[i]->urlId, url_uuid); 
        printf("%-45s %-24s\n", url_uuid, (*p)[i]->url);
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

    sqlite3_step(sql);
    sqlite3_close(db);

    return TRUE;
}



/**
	Multi threaded downloading code for youtube url grabber
*/

thread_arg *buildThreadArgument(url_t **subArr, int size, int tid){
    thread_arg *newThreadArg = (thread_arg*)malloc(sizeof(thread_arg));
    newThreadArg->urlSubArr = subArr;
    newThreadArg->urlLimit = size;
    newThreadArg->threadId = tid;
    return newThreadArg;
}


void appendThreadArg(thread_arg **headRef, thread_arg *newThread){
    newThread->nextThread = (*headRef);
    (*headRef) = newThread;
}


void printThreadUrls(thread_arg *threadArg){
    printf("====================\n");
    url_t ***p = &threadArg->urlSubArr;
    for(int i = 0; i < threadArg->urlLimit; i++){
        dlog("URL THREAD", (*p)[i]->url);
    }
    printf("====================\n");
    dlog_int("URL SIZE", threadArg->urlLimit);
    dlog_int("THREAD ID", threadArg->threadId);
}



void downloadBackup(url_t **urls, int size){

    if(chdir(YOUTUBE_FILE_PATH) != 0){
        printf("ERROR CHANGE DIR YOUTUBE \n");
        perror("chdir() to /error failed");
    }

    url_t ***p = &urls;
    printf("=================\n");
    for(int i = 0; i < size; i++){
        dlog("URL DOWNLOAD", (*p)[i]->url);
        char buffer[500];
        sprintf(buffer, "python3 yt.py %s", (*p)[i]->url);
        system(buffer);
    }

}


void *downloadUrlThread(void *url_struct){
    thread_arg *args = (thread_arg*)url_struct;
    dlog_int("THREAD ID", args->threadId);
    downloadBackup(args->urlSubArr, args->urlLimit);
}


void spawnThreadArguments(thread_arg *headThreadArg){
    while(headThreadArg != NULL){
        dlog("THREAD SPAWN", "SPAWNED URL THREAD");
        pthread_create(&headThreadArg->urlThread, NULL, downloadUrlThread, headThreadArg);
        headThreadArg = headThreadArg->nextThread;
    }
}


void joinThreadArguments(thread_arg *headThreadArg){
    while(headThreadArg != NULL){
        dlog("THREAD JOIN", "JOINING URL THREADS");
        pthread_join(headThreadArg->urlThread, NULL);
        headThreadArg = headThreadArg->nextThread;
    }
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
    dlog("LOGGING", "SUB ARRAY PASSED TO THREAD");
    for(int i = 0; i < arrSize; i++){
        dlog("SUB ARRAY", (*p)[i]->url);
    }
}



void grabDatabaseUrls(int threadCount){

    thread_arg *head = NULL;
    int urlLimit = getUrlTableSize();
    url_t **urls = initUrls(urlLimit);
    url_t ***p = &urls;

    printf("URL SIZE:%d \n", urlLimit);

    int start, split, end, indexCount;
    for(int i = 0; i < urlLimit; i+= threadCount){
        start = i; 
        split = i + threadCount;
        end = urlLimit - 1; 
        
        url_t **urlSubArr = allocateSubArray(threadCount);
        indexCount = 0; 

        for(int j = start; j < split; j++){
            urlSubArr[indexCount] = (*p)[j];
            indexCount += 1; 
        }   

        // account for remainder
        if((end-start)+1 < threadCount){
            int remainder = (end-start)+1;
            dlog_int("REMAINDER COUNT", remainder);
            indexCount = 0;
            url_t **remainderSubArr = allocateSubArray(remainder);
            for(int j = split; j <= end; j++){
                remainderSubArr[indexCount] = (*p)[j];
                indexCount += 1;
            }   
            dlog("YOUTUBE SUBROUTINE", "REMAINDER THREAD COUNT");
            printSubArray(urlSubArr, remainder);
            thread_arg *remainderThread = buildThreadArgument(remainderSubArr, remainder, 0);
            appendThreadArg(&head, remainderThread);
            break;  
        }else{
            printSubArray(urlSubArr, threadCount);
            thread_arg *newThread = buildThreadArgument(urlSubArr, threadCount, i);
            appendThreadArg(&head, newThread); 
        }       
    }

    spawnThreadArguments(head);
    joinThreadArguments(head); 
    
}

