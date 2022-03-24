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
	endpoint_t *e6 = createEndpoint(
                        "sync-songs",
                        "sync",
                        "sync audio files in directory to database"
                    );
    e6->endpointLogic =  syncAudioFilesToDb;
    e6->testEndpointLogic = testSyncAudioFilesToDb;
    appendEndpoint(&head, e6);


	// delete songs
	endpoint_t *e5 = createEndpoint(
                        "delete-songs",
                        "das",
                        "delete all songs in database"
                    );
    e5->endpointLogic = deleteAllSongsCmd;
    e5->testEndpointLogic = testDeleteSongs;
    appendEndpoint(&head, e5);
	
	// delete song
	endpoint_t *e4 = createEndpoint(
                        "delete-song",
                        "ds",
                        "delete song by name"
                    );
    e4->endpointLogic = deleteSongCmd;
    e4->testEndpointLogic = testDeleteSong;
    appendEndpoint(&head, e4);


	// update song
	endpoint_t *e3 = createEndpoint(
                        "update-song",
                        "us",
                        "update song names on dtunes"
                    );
	
	e3->endpointLogic = updateSongCmd; 
	e3->testEndpointLogic = testUpdateSong; 
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
	endpoint_t *e7 = createEndpoint(
                        "delete-playlists",
                        "dap",
                        "delete all playlists in database"
                    );
    e7->endpointLogic = deleteAllPlaylistsCmd;
    e7->testEndpointLogic = testDeletePlaylists;
    appendEndpoint(&head, e7);

	
	// delete song
	endpoint_t *e8 = createEndpoint(
                        "delete-playlist",
                        "dp",
                        "delete playlist by name"
                    );
    e8->endpointLogic = deletePlaylistCmd;
    e8->testEndpointLogic = testDeletePlaylist;
    appendEndpoint(&head, e8);

	
	// view songs
	endpoint_t *e9 = createEndpoint(
                        "view-playlists",
                        "vp",
                        "view all playlists on dtunes"
                    );
    e9->endpointLogic = viewPlaylistCmd;
    e9->testEndpointLogic = testViewPlaylists;
    appendEndpoint(&head, e9);

	
	// create song
	endpoint_t *e10 = createEndpoint(
                        "create-playlist",
                        "cp",
                        "create playlist on dtunes"
                    );
    e10->endpointLogic = insertPlaylistCmd;
    e10->testEndpointLogic = testInsertPlaylist;
    appendEndpoint(&head, e10);

	

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

	execEndpoints(argc, argv, head); 

	


	return 0; 
}	
