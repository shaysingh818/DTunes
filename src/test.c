/**
	This file is meant to act as an isolation unit for testing code. When shipping a new feature, put all functions and header file attributes in here. No other headerfiles should be compiled with this code. Overall, this file is meant to be a place to expirement with code that could be added to the main code base. 
*/


#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>
#include <sqlite3.h>
#include <time.h>
#include <pthread.h> 

#define DB_PATH "../data/db/dtunes.db"
#define AUDIO_FILE_PATH  "../data/audiofiles"
#define YOUTUBE_FILE_PATH  "../data/youtubedl"

// queries needed
#define VIEW_DB_URLS "SELECT * FROM YOUTUBE_URL"
#define COUNT_DB_URLS "SELECT COUNT(*) FROM YOUTUBE_URL"

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


struct YouTubeUrl {
    int url[100]; 
    int dateCreated[30]; 
};

typedef struct YouTubeUrl url_t;


// thread argument
struct UrlThread {
	url_t** urlSubArr; 
	int urlLimit; 
}; 

typedef struct UrlThread url_thread_t; 


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




void* downloadThread(void *my_struct){
	url_thread_t *args = (url_thread_t*)my_struct;
	for(int i = 0; i < args->urlLimit; i++){
		url_t ***p = &args->urlSubArr;
		printf("%d\n", (*p)[i]->limit);  
	}
}


void downloadUrls(int threadCount){

	pthread_t myThread[2]; 

	// create 4 random numbers in sub array
	url_t **testUrls = malloc(5 * sizeof(url_t*)); 
	for(int i = 0; i < 5; i++){
		testUrls[i] = malloc(sizeof(url_t)); 
	}

	for(int j = 0; j < 5; j++){
		// populate the struct		
		testUrls[j]->arr = 10; 
		testUrls[j]->limit = 10; 
	}

	url_thread_t threadArg; 
	threadArg.urlSubArr = testUrls; 
	threadArg.urlLimit = 5; 	

	// create two threads
	for(int i = 0; i < 2; i++){
		pthread_create(&myThread[i], NULL, downloadThread, &threadArg); 
	}

	// join two threads	
	for(int i = 0; i < 2; i++){
		pthread_join(myThread[i], NULL); 
	}


}



void downloadUrls(url_t** ytUrls, int urlSize, int threadCount){

    pthread_t urlThreads[threadCount];
    url_t ***p = &ytUrls;

    int start, split, end, indexCount;
    for(int i = 0; i < urlSize; i+=threadCount){

        start = i;
        split = i + threadCount;
        end = urlSize - 1;
        url_t **urlSubArr = allocateSubArray(threadCount);
        indexCount = 0;

        for(int j = start; j < split; j++){
            urlSubArr[indexCount] = (*p)[j];
            indexCount += 1;
        }

        /*url_thread_t threadArg;
        threadArg->urlSubArr = urlSubArr;
        threadArg->urlLimit = threadCount;

        printSubArray(urlSubArr, threadCount);
        pthread_create(&urlThreads[i], NULL, youtubeDownloadBackup, urlThread);

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
        }*/ 

    }
}




int main(int argc, char* argv[]){


	// load urls

}
