#include "libs/db.h"
#include "libs/urls.h"



struct ThreadArg {
    url_t** urlSubArr;
	int urlLimit; 
	int threadId;
	pthread_t urlThread; 
	struct ThreadArg* nextThread;  
};

typedef struct ThreadArg thread_arg;


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
		printf("Url thread arg: %s\n", (*p)[i]->url); 
	}
	printf("====================\n"); 
	printf("Url Size: %d\n", threadArg->urlLimit);  
	printf("Thread Id: %d\n", threadArg->threadId);  
}


void downloadBackup(url_t **urls, int size){

	if(chdir(YOUTUBE_FILE_PATH) != 0){
        printf("ERROR CHANGE DIR YOUTUBE \n");
        perror("chdir() to /error failed");
    }
	
	url_t ***p = &urls;
	printf("=================\n"); 
	for(int i = 0; i < size; i++){
		printf("Url: %s\n", (*p)[i]->url);
		char buffer[500]; 
		sprintf(buffer, "python3 yt.py %s", (*p)[i]->url); 
		system(buffer);  
	}

}


void *downloadUrlThread(void *url_struct){
	thread_arg *args = (thread_arg*)url_struct;
	printf("THREAD ID: %d\n", args->threadId);
	downloadBackup(args->urlSubArr, args->urlLimit);  
}


void spawnThreadArguments(thread_arg *headThreadArg){
	while(headThreadArg != NULL){	
		printf("Created thread argument\n"); 
		pthread_create(&headThreadArg->urlThread, NULL, downloadUrlThread, headThreadArg); 
		headThreadArg = headThreadArg->nextThread; 
	}
}


void joinThreadArguments(thread_arg *headThreadArg){
	while(headThreadArg != NULL){	
		printf("Created thread argument\n"); 
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
	printf("Sub array urls passed to thread: \n");
    printf("[ ");
    for(int i = 0; i < arrSize; i++){
        printf("%s ",(*p)[i]->url);
    }
    printf("]\n");
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
			printf("remainder: %d\n", remainder); 
			indexCount = 0;
            url_t **remainderSubArr = allocateSubArray(remainder);
            for(int j = split; j <= end; j++){
                remainderSubArr[indexCount] = (*p)[j];
                indexCount += 1;
            }	
			printf("remainder thread count:\n"); 
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


int main(int argc, int **argv){

	// small todos
	// 1. use logging library functions 
	// 2. make delete youtube url work (DTunes db method)
	// 3. 

	grabDatabaseUrls(2); 
	/**
	thread_arg *head = NULL; 
	// get urls from lib
	int urlLimit = getUrlTableSize(); 
	url_t **urls = initUrls(urlLimit);
	url_t ***p = &urls;

	printf("%d \n", urlLimit);

	int threadCount = 2; 
	pthread_t urlThreads[threadCount];
	thread_arg threadArgArray[threadCount]; 

	int start, split, end, indexCount, threadCounter = 0; 
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
			
		printSubArray(urlSubArr, threadCount);
		thread_arg *newThread = buildThreadArgument(urlSubArr, threadCount, i); 
		appendThreadArg(&head, newThread); 
	}


	// traverse thread list and spawn
	spawnThreadArguments(head); 	
	joinThreadArguments(head); 
	*/




}
