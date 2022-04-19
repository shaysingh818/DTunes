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


void d_log(char* type, char* message){
    char* currTime = getCurrentTime() + '\0';
    printf("[%s]: %s\n",  type, message);
}

// logging functions
void dlog(char *type, char *message){
    printf("[%s]: %s\n", type, message);
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




url_t **initUrls(int limit){
	// allocate space for url array
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



void backupVideo(char *dbUrl, char *filePath){
    char buffer[500];
    sprintf(buffer, "python3 %s/yt.py %s", filePath, dbUrl);
    dlog("COMMAND", buffer);
    system(buffer);
}

/**
	Function that will be threaded should take in a list of urls. The amount of urls are arbitary, but the function
	should be able to execute in parallel with other instances of the function

*/

void *downloadThread(void *vargp){
	// taking in num as param
	int *count = (int*)vargp; 
	sleep(1); 
	printf("Threaded function %d\n", *count); 
	return NULL; 
}


void downloadUrls(url_t** ytUrls, int urlSize, int threadCount){

	pthread_t urlThreads[threadCount];
	url_t ***p = &ytUrls; 

	int start, split, end; 
	for(int i = 0; i < urlSize; i+=threadCount){
		start = i; 
		split = i + threadCount; 
		end = urlSize - 1; 
		printf("["); 	
		url_t **urlSubArr = malloc(threadCount * sizeof(url_t*)); 
		for(int j = start; j < split; j++){
			urlSubArr[i] = malloc(sizeof(url_t)); 
			urlSubArr[i] = (*p)[j]; 	
		}
		printf("]\n"); 
		
		if(end-split < threadCount){
			printf("["); 
			for(int j = split; j <= end; j++){
				printf("%s ", (*p)[j]->url); 
			}
			printf("]\n"); 
			break; 
		}
	
		if(split == end){
			break; 
		}
		
	}
}

/**
	This is the function that divides the input urls into parallel tasks
	If numThreads=4, then it will divide the url array into 4 sections
*/
void startDownloadBackUp(url_t** ytUrls, int urlSize,  int numThreads){
	pthread_t urlThreads[numThreads];

	// [0,1,2,3,4,5,6,7,8,9,10] -> [0,1,2,3] [4,5,6,7] [8,9,10]
	// divide the ytUrls into sections, sections are made depending on what the numThreads var is. 
	// pass each section into individual thread when going through numThreads
	url_t ***p = &ytUrls; 
	int test[22] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17, 18, 19, 20, 21}; 
	// subarray algorithm
	int start, end, split;
	for(int i = 0; i < sizeof(test)/sizeof(test[0]); i+=numThreads){
		start = i;
		split = i + numThreads; 
		end = sizeof(test)/sizeof(test[0]) - 1;
		//printf("%d : %d \n", start, split);
		// get remainder
		printf("["); 
		for(int j = start; j < split; j++){
			printf("%d ", test[j]); 
		}
		printf("]\n"); 
 
		if(end-split < numThreads){
			int first = split; 
			int second = end; 
			//printf("Difference: %d : %d\n", first, second);
			printf("["); 
			for(int j = split; j <= end; j++){
				printf("%d ", test[j]); 
			}
			printf("]\n"); 
			break;  
		}
		// if perfect split
		if(split == end){
			break; 
		}
	} 
		

	
	/**
	//url_t **urls = (url_t **)malloc(urlsSize * sizeof(url_t *)); 
	// allocate threads
	for(int i = 0; i < numThreads; i++){
		pthread_create(&urlThreads[i], NULL, downloadThread, (void *)&i); 
	}
	pthread_exit(NULL); 
	*/ 
}


int main(int argc, char* argv[]){

	printf("Testing code \n"); 

	/**
	pthread_t thread_id, thread_id2; 
	pthread_create(&thread_id, NULL, downloadThread, "testing message"); 
	pthread_create(&thread_id2, NULL, downloadThread, "testing message 2"); 
	pthread_join(thread_id, NULL); 
	pthread_join(thread_id2, NULL); 
	*/
	int urlLimit = getUrlTableSize();
	url_t **urls = initUrls(urlLimit);
	downloadUrls(urls, urlLimit, 4); 

	exit(0);
	

}
