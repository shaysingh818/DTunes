#include "libs/db.h"
#include "libs/urls.h"



struct ThreadArg {
    url_t** urlSubArr;
	int urlLimit; 
};

typedef struct ThreadArg thread_arg;



void *downloadUrlThread(void *url_struct){
	thread_arg *args = (thread_arg*)url_struct;
	/**
	url_t *p = args->urlSubArr;

	if(chdir(YOUTUBE_FILE_PATH) != 0){
        printf("ERROR CHANGE DIR YOUTUBE \n");
        perror("chdir() to /error failed");
    }
 
	char buffer[500]; 
	sprintf(buffer, "python3 yt.py %s", p->url); 
	system(buffer); */
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
    printf("[ ");
    for(int i = 0; i < arrSize; i++){
        printf("%s ",(*p)[i]->url);
    }
    printf("]\n");
}


int main(int argc, int **argv){


	// get urls from lib
	int urlLimit = getUrlTableSize(); 
	url_t **urls = initUrls(urlLimit);
	url_t ***p = &urls;

	printf("%d \n", urlLimit);

	int threadCount = 2; 
	pthread_t urlThreads[threadCount];

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
		
		// throw subarray to thread
		thread_arg urlThread;
		urlThread.urlSubArr = urlSubArr; 
		urlThread.urlLimit = threadCount;  

		printSubArray(urlSubArr, threadCount);
		pthread_create(&urlThreads[i], NULL, downloadUrlThread, &urlThread);  

	}

	// split into 2  chunks (to confirm that it happening at the same time)
	// join all the threads (Finish tomorrow)

	
	
}
