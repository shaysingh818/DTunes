#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>
#include <sqlite3.h>

#define YOUTUBE_FILE_PATH  "../data/youtubedl"

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


// logging functions
void dlog(char *type, char *message){
    printf("[%s]: %s\n", type, message);
}


int downloadVideo(char *url){
    char buffer[500];
    if(chdir(YOUTUBE_FILE_PATH) != 0){
        dlog("ERROR", "CHANGE DIR YOUTUBE");
		perror("chdir() to /error failed");  
        return FALSE;
    } 
    sprintf(buffer, "python3 yt.py %s", url);
    dlog("COMMAND", buffer);
    system(buffer);
    return TRUE;
}


int main(int argc, char* argv[]){

	if(argc < 1){
		printf("Not enough args supplied \n"); 
	}else{
		downloadVideo(argv[1]); 
	}

}
