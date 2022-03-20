#include "endpoints.h"


#define TRUE 1
#define FALSE 0
#define DEBUG FALSE

// head endpoint node
endpoint_t *head = NULL;


void printDTunesBanner(FILE *fptr){
    char read_string[128];
    while(fgets(read_string,sizeof(read_string),fptr) != NULL){
        printf("%s",read_string);
	}
}


void songEndpoints(){
		
	
	// sync songs in directory
	endpoint_t *e9 = createEndpoint(
                        "sync-songs",
                        "sync",
                        "sync audio files in directory to database"
                    );
    e9->endpointLogic =  syncAudioFilesToDb;
    //e9->testEndpointLogic = NULL;
    appendEndpoint(&head, e9);

	// delete songs
	endpoint_t *e4 = createEndpoint(
                        "delete-songs",
                        "ds",
                        "delete all songs in database"
                    );
    e4->endpointLogic = deleteAllSongsCmd;
    e4->testEndpointLogic = testDeleteSongs;
    appendEndpoint(&head, e4);
	
	// delete song
	endpoint_t *e3 = createEndpoint(
                        "delete-song",
                        "ds",
                        "delete song by name"
                    );
    e3->endpointLogic = deleteSongCmd;
    e3->testEndpointLogic = testDeleteSong;
    appendEndpoint(&head, e3);

	
	// view songs
	endpoint_t *e2 = createEndpoint(
                        "view-songs",
                        "vs",
                        "view all songs on dtunes"
                    );
    e2->endpointLogic = viewSongsCmd;
    e2->testEndpointLogic = testViewSongs;
    appendEndpoint(&head, e2);

	
	// create song
	endpoint_t *e1 = createEndpoint(
                        "create-song",
                        "cs",
                        "create song on dtunes using youtube url"
                    );
    e1->endpointLogic = insertSongCmd;
    e1->testEndpointLogic = testInsertSong;
    appendEndpoint(&head, e1);

	

}


void playlistEndpoints(){

		

	// delete songs
	endpoint_t *e8 = createEndpoint(
                        "delete-playlists",
                        "dap",
                        "delete all playlists in database"
                    );
    e8->endpointLogic = deleteAllPlaylistsCmd;
    e8->testEndpointLogic = testDeletePlaylists;
    appendEndpoint(&head, e8);

	
	// delete song
	endpoint_t *e7 = createEndpoint(
                        "delete-playlist",
                        "dp",
                        "delete playlist by name"
                    );
    e7->endpointLogic = deletePlaylistCmd;
    e7->testEndpointLogic = testDeletePlaylist;
    appendEndpoint(&head, e7);

	
	// view songs
	endpoint_t *e6 = createEndpoint(
                        "view-playlists",
                        "vp",
                        "view all playlists on dtunes"
                    );
    e6->endpointLogic = viewPlaylistCmd;
    e6->testEndpointLogic = testViewPlaylists;
    appendEndpoint(&head, e6);

	
	// create song
	endpoint_t *e5 = createEndpoint(
                        "create-playlist",
                        "cp",
                        "create playlist on dtunes"
                    );
    e5->endpointLogic = insertPlaylistCmd;
    e5->testEndpointLogic = testInsertPlaylist;
    appendEndpoint(&head, e5);

	

}


int main(int argc, char* argv[]){
	
	// argument debugger
	if(DEBUG == 1){
		printf("Arg 0: %s\n", argv[0]); 
		printf("Arg 1: %s\n", argv[1]); 
		printf("Arg 2: %s\n", argv[2]);
	}

	// configure endpoints
	playlistEndpoints(); 
	songEndpoints(); 

	if(strcmp(argv[1], "help") == 0){
		printEndpoints(head); 
	}

	execEndpoints(argv, head); 

	


	return 0; 
}	
