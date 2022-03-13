#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>
#include <sqlite3.h>

#define TRUE 1
#define FALSE 0

#define DEBUG FALSE


// logging functions
void dlog(char *type, char*message){
    printf("[%s]: %s\n", type, message);
}


void dlog_int(char *type, int message){
    printf("[%s]: %d\n", type, message);
}

void dlog_counter(char *type, char *message, int counter){
	printf("[%d]: %s  %s\n", counter, type, message); 
}


void d_log_time(char* type, char* message){ 
	time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    printf("[%s][%s]: %s\n",  time_str, type, message);
}


char* combineFileStrs(const char *cwd, const char *fileName){
    char *result = malloc(strlen(cwd) + strlen(fileName) + 1);
    strcpy(result, cwd);
    strcat(result, fileName);
    return result;
}


// utility functions for syncing meta data to db
char *currentTime(){	
	time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
	return time_str; 
}


int main(){
	
}
