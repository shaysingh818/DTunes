#include "endpoints.h"


#define TRUE 1
#define FALSE 0
#define DEBUG FALSE



endpoint_t **songEndpoints(char* argv[]){

	endpoint_t **endpoints = malloc(4 * sizeof(endpoint_t*));
	 for(int i = 0; i < 4; i++){
        endpoints[i] = malloc(sizeof(endpoint_t)); 
    }

	// Create song
	endpoint_t *e1 = createEndpoint("create-song", "cs");
    e1->endpointLogic = insertSongCmd; // attach here
	e1->testEndpointLogic = testInsertSong;
    setDocumentationString(e1, "create song");
    e1->endpointLogic(e1, argv[1], argv[2]);
	endpoints[0] = e1; 

	
	// View songs
	endpoint_t *e2 = createEndpoint("view-song", "vs");
    e2->endpointLogic = viewSongsCmd; // attach here	
	e2->testEndpointLogic = testViewSongs;
    setDocumentationString(e2, "view song");
    e2->endpointLogic(e2, argv[1], NULL); 
	endpoints[1] = e2; 

	
	// Delete playlist
	endpoint_t *e3 = createEndpoint("delete-song", "ds");
    e3->endpointLogic = deleteSongCmd; // attach here
    e3->testEndpointLogic = testDeleteSong; // attach here
    setDocumentationString(e3, "delete a song");
    e3->endpointLogic(e3, argv[1], argv[2]);
	endpoints[2] = e3; 

	
	// Delete all playlist
	endpoint_t *e4 = createEndpoint("delete-all-songs", "das");
    e4->endpointLogic = deleteAllSongsCmd; // attach here
    e4->testEndpointLogic = testDeleteSongs; // attach here
    setDocumentationString(e4, "delete all songs");
    e4->endpointLogic(e4, argv[1], NULL);
	endpoints[3] = e4; 

	return endpoints; 
}


endpoint_t **playlistEndpoints(char* argv[]){

	endpoint_t **endpoints = malloc(4 * sizeof(endpoint_t*));
	 for(int i = 0; i < 4; i++){
        endpoints[i] = malloc(sizeof(endpoint_t)); 
    }

	// Create playlist
	endpoint_t *e1 = createEndpoint("create-playlist", "cp");
    e1->endpointLogic = insertPlaylistCmd; // attach here	
    e1->testEndpointLogic = testInsertPlaylist; // attach here
    setDocumentationString(e1, "create playlist");
    e1->endpointLogic(e1, argv[1], argv[2]);
	endpoints[0] = e1; 

	
	// View playlist
	endpoint_t *e2 = createEndpoint("view-playlist", "vp");
    e2->endpointLogic = viewPlaylistCmd; // attach here	
    e2->testEndpointLogic = testViewPlaylists; // attach here
    setDocumentationString(e2, "viewplaylist");
    e2->endpointLogic(e2, argv[1], NULL); 
	endpoints[1] = e2; 

	
	// Delete playlist
	endpoint_t *e3 = createEndpoint("delete-playlist", "dp");
    e3->endpointLogic = deletePlaylistCmd; // attach here	
    e3->testEndpointLogic = testDeletePlaylist; // attach here
    setDocumentationString(e3, "delete a playlist");
    e3->endpointLogic(e3, argv[1], argv[2]);
	endpoints[2] = e3; 

	
	// Delete all playlist
	endpoint_t *e4 = createEndpoint("delete-all-playlist", "dap");
    e4->endpointLogic = deleteAllPlaylistsCmd; // attach here
    e4->testEndpointLogic = testDeletePlaylists; // attach here
    setDocumentationString(e4, "delete all playlists");
    e4->endpointLogic(e4, argv[1], NULL);
	endpoints[3] = e4; 

	return endpoints; 
}


int main(int argc, char* argv[]){

	
	endpoint_t **playlistService = playlistEndpoints(argv); 
	endpoint_t **songService = songEndpoints(argv); 

	// argument debugger
	if(DEBUG == 1){
		printf("Arg 0: %s\n", argv[0]); 
		printf("Arg 1: %s\n", argv[1]); 
		printf("Arg 2: %s\n", argv[2]);
	}

	
	if(strcmp(argv[1], "help") == 0){
		// columns	
		char *column1 = "FLAG-COMMAND"; 
		char *column2 = "DOCUMENTATION";
		// display 
		printf("\e[0;31m");	
		printf("%c[1m",27);
		printf("%-25s  %-30s\n", column1, column2);
		printf("==========================================================\n");
		printEndpointHelpMenu(playlistService, 4, argv);
		printEndpointHelpMenu(songService, 4, argv); 
	}


	return 0; 
}	
