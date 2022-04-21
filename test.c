#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


// test model for the orm
struct Song {
	char name[30];	
	char artist[30]; 
	char dateCreated[30]; 
}; 

typedef struct Song song_t; 



int main(int argc, char* argv[]){

	DIR *audioFolder = opendir("data/audiofiles");
	struct dirent *next_file; 
	char filepath[300];

	while((next_file=readdir(audioFolder)) != NULL){
		sprintf(filepath, "%s/%s", "data/audiofiles", next_file->d_name); 
		remove(filepath); 
	}
	closedir(audioFolder); 
	return 0; 
}
