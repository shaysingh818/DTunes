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

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE

struct Url {
	char url[100]; 
}; 

typedef struct Url url_t;


struct ThreadArg {
    url_t* urlSubArr;
};

typedef struct ThreadArg url_thread_t;



void *downloadUrlThread(void *url_struct){
	url_thread_t *args = (url_thread_t*)url_struct;
	char buffer[500]; 
	sprintf(buffer, "python3 yt.py %s", url_struct->url); 
	system(buffer); 
}


int main(int argc, int **argv){

	url_t *myUrl = malloc(sizeof(url_t)); 
	strcpy(myUrl->url, "https://www.youtube.com/watch?v=Jo2QHf4Itoo"); 

	
	url_t *myUrl2 = malloc(sizeof(url_t)); 
	strcpy(myUrl2->url, "https://www.youtube.com/watch?v=Jo2QHf4Itoo"); 
	
	pthread_t urlThread; 
	thread_arg threadArg;
    threadArg.urlSubArr = myUrl;

	
	pthread_t urlThread2; 
	thread_arg threadArg2;
    threadArg2.urlSubArr = myUrl2;

	pthread_create(&urlThread, NULL, downloadUrlThread, &threadArg); 
	pthread_create(&urlThread2, NULL, downloadUrlThread, &threadArg2);

	pthread_join(&urlThread, NULL); 
	pthread_join(&urlThread2, NULL); 
	
}
