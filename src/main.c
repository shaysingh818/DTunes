#include "endpoints.h"
#include "unit_tests.h"


#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


void playlistEndpoints(char* argv[]){
	
	// Create playlist
	endpoint_t *e1 = createEndpoint("create-playlist", "cp");
    e1->endpointLogic = insertPlaylistCmd; // attach here
    setDocumentationString(e1, "create playlist");
    e1->endpointLogic(e1, argv[1], argv[2]);

	
	// View playlist
	endpoint_t *e2 = createEndpoint("view-playlist", "vp");
    e2->endpointLogic = viewPlaylistCmd; // attach here
    setDocumentationString(e2, "viewplaylist");
    e2->endpointLogic(e2, argv[1], NULL); 

	
	// Delete playlist
	endpoint_t *e3 = createEndpoint("delete-playlist", "dp");
    e3->endpointLogic = deletePlaylistCmd; // attach here
    setDocumentationString(e3, "delete a playlist");
    e3->endpointLogic(e3, argv[1], argv[2]);

	
	// Delete all playlist
	endpoint_t *e4 = createEndpoint("delete-all-playlist", "dap");
    e4->endpointLogic = deleteAllPlaylistsCmd; // attach here
    setDocumentationString(e4, "delete all playlists");
    e4->endpointLogic(e4, argv[1], NULL);
}


int main(int argc, char* argv[]){

	// argument debugger
	if(DEBUG == 1){
		printf("Arg 0: %s\n", argv[0]); 
		printf("Arg 1: %s\n", argv[1]); 
		printf("Arg 2: %s\n", argv[2]);
	}

	/**
		Main DTUNES user menu
	*/

	//DTunesHelpMenu(argv[1]);

	// DTUNES help endpoint
	endpoint_t *e1 = createEndpoint("dtunes-help", "help");
    e1->endpointLogic = DTunesHelpMenu; // attach here
    setDocumentationString(e1, "help menu for dtunes");
    e1->endpointLogic(e1, argv[1], NULL);

	playlistEndpoints(argv); 

	
	 
	
	// playlists
	//insertPlaylistCmd(argv[1], argv[2]);
	//viewPlaylistCmd(argv[1]);  
	//deletePlaylistCmd(argv[1], argv[2]); 
	//deleteAllPlaylistsCmd(argv[1]); 

	// songs
	//insertSongCmd(argv[1], argv[2]); 
	//viewSongsCmd(argv[1]); 
	//deleteSongCmd(argv[1], argv[2]);	
	//deleteAllSongsCmd(argv[1]);

	
	/**
		DTUNES test suite for testing all features
	*/

	testCaseHelpMenu(argv[1]); 		

	// test cases for playlists	
	testInsertPlaylist(argv[1], argv[2]);	
	testLoadPlaylists(argv[1]); 
	testViewPlaylists(argv[1]); 		
	testDeletePlaylist(argv[1], argv[2]);
	testDeletePlaylists(argv[1]);
	
	// test cases for song	
	testLoadSongs(argv[1]); 
	testUpdateSong(argv[1], argv[2], argv[3]);
	testViewSongs(argv[1]); 
	testInsertSong(argv[1], argv[2]);	
	testDeleteSong(argv[1], argv[2]); 
	testDeleteSongs(argv[1]);

	// sync songs
	syncAudioFiles(argv[1]);	
	testDownloadYTVideo(argv[1], argv[2]);

	runAllTests(argv[1]);  

	return 0; 
}	
