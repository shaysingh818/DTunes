#ifndef URLS_H
#define URLS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h> 

// db model
struct YouTubeUrl {
    char url[100];
    char dateCreated[30];
};

typedef struct YouTubeUrl url_t;


// throwing arguments in a struct
struct UrlThread {
	url_t** urlSubArr;
	int urlLimit;  
}; 

typedef struct UrlThread url_thread_t; 


// db functions
url_t **initUrls(int limit);
int getUrlTableSize();
int insertUrl(char *urlString, char *currentTime);
int createYoutubeUrl(url_t* newUrl);
int viewUrls();
int updateUrl(char *prevUrl, char *newUrl);
int deleteYoutubeUrl(char *url);
int deleteAllYoutubeUrls();
int checkUrlExist(char *url);


// functions involving youtube url scraping
void youtubeDownloadBackup(void *vargp);
url_t **allocateSubArray(int threadCount); 
void printSubArray(url_t **subArr, int arrSize); 
void downloadUrls(url_t** ytUrls, int urlSize, int threadCount); 


#endif
