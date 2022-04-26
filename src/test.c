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
    char url[100]; 
    char dateCreated[30]; 
};

typedef struct YouTubeUrl url_t;


// thread argument
struct UrlThread {
	url_t** urlSubArr; 
	int urlLimit; 
}; 

typedef struct UrlThread url_thread_t; 


sqlite3* openDB(char *filename){
    sqlite3 *db;
    if(sqlite3_open(filename, &db)){
        printf("Could not open the db : \n ");

        printf("Could not open the db:  %s\n",sqlite3_errmsg(db));
        exit(-1);
    }
    return db;
}


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


void* youtubeDownloadBackup(void *vargp){
	url_thread_t *args = (url_thread_t*)vargp;
	printf("Url limit: %d\n", args->urlLimit);

	if(chdir(YOUTUBE_FILE_PATH) != 0){
        printf("ERROR CHANGE DIR YOUTUBE \n");
        perror("chdir() to /error failed");
    }
	
	for(int i = 0; i < args->urlLimit; i++){
		url_t ***p = &args->urlSubArr;
		printf("%s\n", (*p)[i]->url); 
		char buffer[500];
    	sprintf(buffer, "python3 yt.py %s", (*p)[i]->url);
    	//dlog("COMMAND", buffer);
    	system(buffer);
	}
 
}


void* downloadThread(void *my_struct){
	url_thread_t *args = (url_thread_t*)my_struct;
	for(int i = 0; i < args->urlLimit; i++){
		url_t ***p = &args->urlSubArr;
		printf("%s\n", (*p)[i]->url);  
	}
}


/**
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


} */



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
			printf("%d \n", j); 
            urlSubArr[indexCount] = (*p)[j];
            indexCount += 1;
        }
		printf("====================\n"); 


        if(end-split < threadCount){
            indexCount = 0;
            url_t **remainderSubArr = allocateSubArray(end-split);
            for(int j = split; j <= end; j++){
                remainderSubArr[indexCount] = (*p)[j];
                indexCount += 1;
            }
		
			// create thread for first sub array
			url_thread_t remainderThreadArg; 
			remainderThreadArg.urlSubArr = remainderSubArr; 
			remainderThreadArg.urlLimit = (end-split); 	
				
			int tempLimit = (end-split)+1;
            printSubArray(remainderSubArr, tempLimit);
        	pthread_create(&urlThreads[i], NULL, youtubeDownloadBackup, &remainderThreadArg); 
            break;

        }else{
			
			// create thread for remaining sub array
			url_thread_t threadArg; 
			threadArg.urlSubArr = urlSubArr; 
			threadArg.urlLimit = threadCount; 	
        	pthread_create(&urlThreads[i], NULL, youtubeDownloadBackup, &threadArg); 
            //printSubArray(urlSubArr, threadCount);
		}

        if(split == end){
            break;
        }

		// join all threads
		for(int i = 0; i < threadCount; i++){
        	pthread_join(urlThreads[i], NULL); 
		}
		


    }
}




int main(int argc, char* argv[]){


	// load urls
	int limit = getUrlTableSize(); 
	url_t ** urls = initUrls(limit); 
	downloadUrls(urls, limit, 4); // thread count of 4
	


}
