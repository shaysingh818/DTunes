#include "test.h"
#include "playlist.h"
#include "song.h"
#include "urls.h"
#include "db.h"

/**
	INTERNAL TESTING SUITE FOR DTUNES
	This tests every single cli command
*/


void testOpenDb(){
	sqlite3 *testdb = openDB(DB_PATH); 
	if(testdb){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "OPEN DB METHOD"); 
	}else{
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "OPEN DB METHOD"); 
	}
}

void testCurrentTime(){
	char *currentTime = getCurrentTime(); 
	// time value two
	time_t rawtime;
    struct tm * timeinfo;
    // get time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

	char *testTime = asctime(timeinfo); 
	int result = strcmp(currentTime, testTime); 
	if(result == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "GET CURRENT TIME"); 
	}else{	
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "GET CURRENT TIME"); 
	}
}


void testCombineFileStrs(){

	char testStr[10] = "hello"; 
	char testStr2[10] = "world"; 
	char restStr[20] = "helloworld"; 
	char *testMethod = combineFileStrs(testStr, testStr2); 
	 
	int result = strcmp(testMethod, restStr); 
	if(result == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "COMBINE STR METHOD"); 
	}else{	
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "COMBINE STR METHOD"); 
	}

}


void testRemoveChar(){

	char testStr[10] = "hello";
	char resultStr[10] = "hell";  
	removeChar(testStr, 'o'); 
	
	int result = strcmp(testStr, resultStr); 
	if(result == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "REMOVE CHARACTER"); 
	}else{	
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "REMOVE CHARACTER"); 
	}
}



void testRenameFile(){

	DIR *folder;
    struct dirent *entry;
    int files = 0;

    folder = opendir("../data/testing");
	 if(folder == NULL){
        perror("Unable to read directory\n");
    }


	// change to desired directory
    if(chdir("../data/testing") != 0){
        dlog("ERROR", "CHANGE DIR");
    }

	system("touch testingfile.txt"); 
	renameFile("testingfile.txt", "newfile.txt"); 

	while((entry=readdir(folder))){
        files++;
        char *tempFile = entry->d_name;
		int result = strcmp(tempFile, "newfile.txt"); 
		if(result == 0){
			printf("\033[0;32m"); // green for pass
			dlog("PASSED", "RENAME FILE"); 
		}else{	
			printf("\e[0;31m"); // red for fail
			dlog("FAILED", "RENAME FILE"); 
		}
			
    }

    closedir(folder);
	system("rm newfile.txt"); 
}


void runAllTests(){

	// test database library
	testOpenDb(); 
	testCurrentTime(); 
	testCombineFileStrs(); 
	testRemoveChar(); 
	testRenameFile(); 

	// testing playlists


	// testing songs


	// testing urls


	// testing url downloader

}
