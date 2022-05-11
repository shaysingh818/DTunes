#include "urls_test.h"
#include "urls.h"
#include "../db/db.h"

void testCreateUrl(){

    char *mytime = getCurrentTime();
    url_t *newUrl;
    newUrl = (url_t*)malloc(sizeof(url_t));

    // set values
    strcpy(newUrl->url, "https://youtube.com"); // no url validation yet
    strcpy(newUrl->dateCreated, mytime);

    int dbResult = createYoutubeUrl(newUrl);
    if(dbResult){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "INSERT URL");
    }else{
        printf("\033[0;31m"); // red for fail
        dlog("FAILED", "INSERT URL");
    }
}


void testDeleteUrl(){
    int dbResult = deleteYoutubeUrl("https://youtube.com");
    if(dbResult){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE URL");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE URL");
    }
}



void testDeleteAllUrls(){

    int dbResult = deleteAllYoutubeUrls();
    if(!dbResult){
        printf("\033[0;31"); // red for fail
        dlog("FAILED", "DELETE ALL URLS");
    }

    int urlResult = getUrlTableSize();
    if(urlResult == 0){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE ALL URLS");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE ALL URLS");
    }
}



void testLoadUrls(){

	char *mytime = getCurrentTime(); 
	// allocate space for three playlists
	url_t **temp_urls = malloc(3 * sizeof(url_t*));
	for(int i = 0; i < 3; i++){
		temp_urls[i] = malloc(sizeof(url_t));  
	}

	strcpy(temp_urls[0]->url, "https://www.youtube.com"); 	
	strcpy(temp_urls[0]->dateCreated, mytime);
	 
	strcpy(temp_urls[1]->url, "https://www.google1.com"); 	
	strcpy(temp_urls[1]->dateCreated, mytime); 
		
	strcpy(temp_urls[2]->url, "https://www.youtubeone.com"); 	
	strcpy(temp_urls[2]->dateCreated, mytime); 

	// loop and insert all of them
	for(int j = 0; j < 3; j++){
		int insertDbResult = createYoutubeUrl(temp_urls[j]);
    	// check result of insert 
    	if(!insertDbResult){
			printf("\033[0;31m"); // green for pass
			dlog("FAILED", "LOAD URLS TEST CASE");
		} 
	}


	// Call the load songs with limit   
    int urlLimit = getUrlTableSize();
    url_t **urls = initUrls(urlLimit);
    url_t ***p = &urls;

    for(int i = 0; i < urlLimit; i++){
		int checkUrl = strcmp(temp_urls[i]->url, (*p)[i]->url); 
		int checkDate = strcmp(temp_urls[i]->dateCreated, (*p)[i]->dateCreated);
		
		if(checkUrl == 0 && checkDate == 0){			
			printf("\033[0;32m"); // green for pass
			dlog("PASSED", "LOAD URLS"); 
		}else{	
			printf("\033[0;31m"); // red for fail
			dlog("FAILED", "LOAD URLS"); 
		}
    }


	int dpResult = deleteAllYoutubeUrls(); 
	if(!dpResult){
		dlog("FAILED", "LOAD URLS (DELETE) "); 
	}
}



void testBuildArgumentThreads(){

	int urlCount = 5; 
	thread_arg *testHead = NULL; 
	// insert some dummy urls
	char *mytime = getCurrentTime();
    // allocate space for three playlists
    url_t **temp_urls = malloc(urlCount * sizeof(url_t*));
    for(int i = 0; i < urlCount; i++){
        temp_urls[i] = malloc(sizeof(url_t));
    }
	
	char tempUrl[40] = "https://www.youtube.com";
	char count[5];  
	for(int i = 0; i < urlCount; i++){
		char buffer[5];
		char tempStr[40]; 
		char tempCount[5]; 
		sprintf(buffer, "%d", i);
		strcpy(tempCount, buffer); 
		strcpy(tempStr, tempUrl); 
		strcat(tempStr, tempCount); 
    	strcpy(temp_urls[i]->url, tempStr);
    	strcpy(temp_urls[i]->dateCreated, mytime);
	}

	  // loop and insert all of them
    for(int j = 0; j < urlCount; j++){
        int insertDbResult = createYoutubeUrl(temp_urls[j]);
        // check result of insert 
        if(!insertDbResult){
            printf("\033[0;31m"); // green for pass
            dlog("FAILED", "BUILD ARGUMENT THREADS");
        }
    }

	int threadCount = 2; // split by 2 threads (without remainder)
    int urlLimit = getUrlTableSize();
    url_t **urls = initUrls(urlLimit);
    url_t ***p = &urls;

	int start, split, end, indexCount = 0; 
	for(int i = 0; i < urlLimit; i += threadCount){
		start = i; 
		split = i + threadCount; 
		end = urlLimit - 1; 

		// subarray
		url_t **subArr = allocateSubArray(threadCount); 	
		// add to sub array
		for(int j = start; j < split; j++){
			subArr[indexCount] = (*p)[j];
			indexCount += 1; 
		}

		thread_arg *newThread = buildThreadArgument(subArr, threadCount, i); 
		appendThreadArg(&testHead, newThread); 
		dlog("PASSED", "ARGUMENT THREADING"); 
	}
}



void runUrlsTest(){
	printf("\033[0;37m");
    dlog("URL DOWNLOAD LIBRARY", "Running tests for url downloading library");
    testCreateUrl();
    testDeleteUrl();
    testDeleteAllUrls();
    testLoadUrls();
    testBuildArgumentThreads();
}

