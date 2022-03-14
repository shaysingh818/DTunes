#include "endpoints.h"
#include "unit_tests.h"


#define TRUE 1
#define FALSE 0
#define DEBUG FALSE




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

	DTunesHelpMenu(argv[1]); 
	
	// playlists
	insertPlaylistCmd(argv[1], argv[2]);
	viewPlaylistCmd(argv[1]);  
	deletePlaylistCmd(argv[1], argv[2]); 
	deleteAllPlaylistsCmd(argv[1]); 

	// songs
	insertSongCmd(argv[1], argv[2]); 
	viewSongsCmd(argv[1]); 
	deleteSongCmd(argv[1], argv[2]);	
	deleteAllSongsCmd(argv[1]);

	
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
