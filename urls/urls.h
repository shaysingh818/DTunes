#ifndef URLS_H
#define URLS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h> 
#include <uuid/uuid.h>


// db model
struct YouTubeUrl {
	uuid_t urlId; 
    char url[100];
    char dateCreated[30];
};

typedef struct YouTubeUrl url_t;


struct ThreadArg {
    url_t** urlSubArr;
    int urlLimit;
    int threadId;
    pthread_t urlThread;
    struct ThreadArg* nextThread;
};

typedef struct ThreadArg thread_arg;


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

// multi threaded download library
thread_arg *buildThreadArgument(url_t **subArr, int size, int tid); 
void appendThreadArg(thread_arg **headRef, thread_arg *newThread); 
void printThreadUrls(thread_arg *threadArg); 
void downloadBackup(url_t **urls, int size); 
void *downloadUrlThread(void *url_struct); 
void spawnThreadArguments(thread_arg *headThreadArg); 
void joinThreadArguments(thread_arg *headThreadArg); 
url_t **allocateSubArray(int threadCount); 
void printSubArray(url_t **subArr, int arrSize); 
void grabDatabaseUrls(int threadCount);





#endif
