#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>
#include <sqlite3.h>
#include "playlist.h"
#include "db.h"

#define TRUE 1
#define FALSE 0

#define DEBUG FALSE

struct Endpoint {
	char name[100];
	char commandLineArg[100]; 
	//void (*logic)(int limit, const struct Endpoint* e); 
};

typedef struct Endpoint endpoint_t;


// function create an endpoint
endpoint_t *createEndpoint(char *name, char *commandLineArg){
	endpoint_t *e1;
    e1 = (endpoint_t*)malloc(sizeof(endpoint_t));
	// bind values
	strcpy(e1->name, name);
    strcpy(e1->commandLineArg, commandLineArg);
	return e1; 	
} 


void insertPlaylistCmd(const struct Endpoint* e, char *myarg, char *myArg2){
	// insert playlist to db
	if(strcmp(myarg, e->name) == 0){

		// bind fields for song model
		char *playlistName = myArg2; 	
		char *mytime = getCurrentTime(); 

		/**
		// create instance of playlist model	
		playlist_t *newPlaylist;
		newPlaylist = (playlist_t*)malloc(sizeof(playlist_t));	
		printf("\033[0;32m");
		printf("[%s]: CREATED INSTANCE OF PLAYLIST:\n", "STRUCTURE ALLOCATION"); 

		// set values
		strcpy(newPlaylist->name, playlistName);	
		strcpy(newPlaylist->dateCreated, mytime);
 
		// insert playlist in database
		int insertDbResult = createPlaylist(newPlaylist);

		// check result of insert 
		if(insertDbResult){
			//printf("\033[0;32m");
			printf("[DTUNES]: Created Playlist\n"); 
		}else{	
			printf("\e[0;31m");
			printf("Something went wrong, refer to test cases\n"); 
		}
		*/
	
	}
}


int main(int argc, char* argv[]){

	
	endpoint_t *e1 = createEndpoint("create-playlist", "cp");
	insertPlaylistCmd(&e1, argv[1], argv[2]);  

	/**
	// create instance of endpoint
	struct Endpoint e1 = {
		"create-song",
		"cs",
		print_endpoint,
		create_song, 
	};

	e1.print(&e1); 
	e1.logic(20, &e1); 
	*/

}
