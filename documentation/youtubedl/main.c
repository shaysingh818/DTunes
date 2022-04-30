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


int main(int argc, int **argv){

	// small todos
	// 1. use logging library functions 
	// 2. make delete youtube url work (DTunes db method)

	double timeSpent = 0.0; 
	clock_t begin = clock(); 

	// time function
	grabDatabaseUrls(3); 

	clock_t end = clock(); 

	timeSpent += (double)(end - begin) / CLOCKS_PER_SEC; 
	printf("The elapsed time is %f seconds", timeSpent); 


}
