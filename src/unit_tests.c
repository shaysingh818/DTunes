#include "unit_tests.h" 
#include "playlist.h"
#include "song.h" 
#include "db.h"


/**
	METHODS FOR TEST CASES

	Test new features in each method, return results of features in functions. 
*/


void printBanner(FILE *fptr){
    char read_string[MAX_LEN];
    while(fgets(read_string,sizeof(read_string),fptr) != NULL){
        printf("%s",read_string);
	}
}

void testCaseHelpMenu(char *myarg){
	
	char *column1 = "FLAG-COMMAND"; 
	char *column2 = "DOCUMENTATION"; 

	if(strcmp(myarg, TEST_HELP_MENU) == 0){
		// help menu for playlists
	
			
		// display banner
		char *filename = "banner.txt";
    	FILE *fptr = NULL;
	
    	if((fptr = fopen(filename,"r")) == NULL){
        	fprintf(stderr,"error opening %s\n",filename);
    	}
    	printBanner(fptr);
		printf("DTUNES testing suite for all methods and features that are being added.: \n\n"); 
    	fclose(fptr);


		// set color	
		printf("\e[0;31m");	
		printf("%c[1m",27);
		printf("%-25s  %-30s\n", column1, column2);
		printf("==========================================================\n"); 
		printf("--%-25s:  test case for inserting a playlist\n", TEST_INSERT_PLAYLIST); 

		printf("--%-25s:  loading playlists into memory\n", TEST_LOAD_PLAYLISTS); 
		printf("--%-25s:  viewing playlists \n", TEST_VIEW_PLAYLISTS); 
		printf("--%-25s:  inserting a playlist\n", TEST_DELETE_PLAYLIST); 
		printf("--%-25s:  inserting a playlist\n", TEST_DELETE_PLAYLISTS); 

		// help menu for songs
		printf("--%-25s:  inserting a song\n", TEST_INSERT_SONG); 
		printf("--%-25s:  loading songs into memory\n", TEST_LOAD_SONGS); 
		printf("--%-25s:  viewing songs\n", TEST_VIEW_SONGS); 
		printf("--%-25s:  deleting songs by name\n", TEST_DELETE_SONG); 
		printf("--%-25s:  delete all songs\n", TEST_DELETE_SONGS);	
		printf("--%-25s:  sync audio file information to sqlite3 database\n", TEST_SYNC_SONGS);	
		printf("--%-25s:  download youtube video\n", TEST_DOWNLOAD_YT);
		printf("\n");  
	}
}

void testInsertPlaylist(char *myarg, char *myarg2){
	if(strcmp(myarg, TEST_INSERT_PLAYLIST) == 0){
	
		// bind fields for song model
		char *playlistName = myarg2; 	
		char *mytime = getCurrentTime(); 

		// create instance of playlist model	
		playlist_t *newPlaylist;
		newPlaylist = (playlist_t*)malloc(sizeof(playlist_t));	
		printf("\033[0;32m\n");
		printf("[%s]: CREATED INSTANCE OF PLAYLIST:\n", "STRUCTURE ALLOCATION"); 

		// set values
		strcpy(newPlaylist->name, playlistName);	
		strcpy(newPlaylist->dateCreated, mytime);

		printf("[%s]: Playlist Name: %s\n", "FIELD DEBUG" ,newPlaylist->name); 
		printf("[%s]: Playlist Date: %s\n", "FIELD DEBUG", newPlaylist->dateCreated);
 
		// insert playlist in database
		int insertDbResult = createPlaylist(newPlaylist);

		// check result of insert 
		if(insertDbResult){
			//printf("\033[0;32m");
			printf("[TEST CASE]: INSERT PLAYLIST: passed\n"); 
		}else{	
			printf("\e[0;31m");
			printf("INSERT PLAYLIST: failed\n"); 
		}
	}
}



void testLoadPlaylists(char *myarg){
	if(strcmp(myarg, TEST_LOAD_PLAYLISTS) == 0){
		// make sure it returns array of song structs
		int playlistLimit = getPlaylistTableSize(); 
		playlist_t **result = initPlaylists(playlistLimit); 
		playlist_t ***p = &result; 
		// test if we can print struct array
		printf("Viewing playlists from struct array: \n"); 
		for(int i = 0; i < playlistLimit; i++){
			printf("=============================================\n"); 
			printf("PLAYLIST NAME: %s\n", (*p)[i]->name); 
			printf("DATE CREATED: %s\n", (*p)[i]->dateCreated); 
		}
	}
}


void testViewPlaylists(char *myarg){
	if(strcmp(myarg, TEST_VIEW_PLAYLISTS) == 0){
		printf("View playlists test case: \n");
		int result = viewPlaylists();  
	}
}


void testDeletePlaylist(char *myarg, char *myarg2){	
	if(strcmp(myarg, TEST_DELETE_PLAYLIST) == 0){
		// test function
		int result = deletePlaylist(myarg2); 
		if(result){
			printf("PASS: Deleted Playlist: %d\n", result); 
		}else{	
			printf("FAIL: Deleted Playlist: %d\n", result); 
		}
	}
}


void testDeletePlaylists(char *myarg){
	if(strcmp(myarg, TEST_DELETE_PLAYLISTS) == 0){
		int result = deleteAllPlaylists(); 
		if(result){
			printf("[TEST CASE]: PASS Deleted ALL playlists: %d\n", result); 
		}else{	
			printf("[TEST CASE] FAIL Deleted ALL playlists: %d\n", result); 
		}
	}
}


void testInsertSong(char *myarg, char *myarg2){
	
	if(strcmp(myarg, TEST_INSERT_SONG) == 0){

		// bind fields for song model
		char *mytime = getCurrentTime(); 
		char *songName = myarg2; 
		char *songPath = "/test/path"; 
		char *songSubs = "test subtitles"; 

		// create instance of playlist model	
		song_t *newSong;
		newSong = (song_t*)malloc(sizeof(song_t));	
		printf("\033[0;32m");
		printf("[STRUCTURE ALLOCATION]: CREATED INSTANCE OF SONG:\n"); 

		// set values
		strcpy(newSong->name, songName);	
		strcpy(newSong->dateCreated, mytime);
		strcpy(newSong->filePath, "test-path");		
		strcpy(newSong->subtitles, "test-titles");	
		newSong->plays = 0;

		// test if attributes were set	
		//printf("\e[0;31m");
		printf("[FIELD DEBUG]: Song Name: %s\n", newSong->name); 
		printf("[FIELD DEBUG]: Song Date: %s\n", newSong->dateCreated); 
		printf("[FIELD DEBUG]: Song Path: %s\n", newSong->filePath); 
		printf("[FIELD DEBUG]: Song Subtitle: %s\n", newSong->subtitles); 	
		printf("[FIELD DEBUG]: Song Plays:  %d\n", newSong->plays); 

		// insert song model struct into db
		int dbResult = createSong(newSong);

		// check if database insert was successful 
		if(dbResult){
			printf("\033[0;32m");
			printf("[TEST CASE]: INSERT SONG: passed \n"); 
		}else{	
			printf("\e[0;31m");
			printf("[TEST CASE]: INSERT SONG: failed\n"); 
		}
	}
}



void testLoadSongs(char *myarg){
	
	if(strcmp(myarg, TEST_LOAD_SONGS) == 0){

		// make sure it returns array of song structs
		int songLimit = getSongTableSize(); 
		song_t **result = initSongs(songLimit); 
		song_t ***p = &result;  
		// test if we can print struct array
		printf("Viewing songs from struct array: \n"); 
		for(int i = 0; i < songLimit; i++){
			printf("=============================================\n"); 
			printf("SONG NAME:%s\n", (*p)[i]->name); 
			printf("DATE CREATED:%s\n", (*p)[i]->dateCreated); 
			printf("FILE PATH:%s\n", (*p)[i]->filePath); 
			printf("SUBTITLES:%s\n", (*p)[i]->subtitles); 
			printf("PLAYS:%d\n", (*p)[i]->plays);
		}


	}

}

void testViewSongs(char *myarg){
	// test view songs		
	if(strcmp(myarg, TEST_VIEW_SONGS) == 0){
		int result = viewSongs();
	}

}



void testDeleteSong(char *myarg, char *myarg2){
	if(strcmp(myarg, TEST_DELETE_SONG) == 0){
		int result = deleteSong(myarg2); 
		if(result){
			printf("[TEST CASE]: PASS Deleted song: %d\n", result); 
		}else{	
			printf("[TEST CASE]: FAIL: Deleted songs: %d\n", result); 
		}
	}
}


void testDeleteSongs(char *myarg){
	if(strcmp(myarg, TEST_DELETE_SONGS) == 0){
		int result = deleteAllSongs(); 
		if(result){
			printf("[TEST CASE]: PASS Deleted ALL songs: %d\n", result); 
		}else{	
			printf("[TEST CASE]: FAIL: Deleted ALL playlists: %d\n", result); 
		}
	}
}



void syncAudioFiles(char *myarg){	
	if(strcmp(myarg, TEST_SYNC_SONGS) == 0){
		//testSync("../data/audiofiles");
		loadAudioFilesFromDirectory(AUDIO_FILE_PATH);  

	}
}



void testDownloadYTVideo(char *myarg, char *myarg2){	
	if(strcmp(myarg, TEST_DOWNLOAD_YT) == 0){
		//testSync("../data/audiofiles");
		int result = downloadVideo(myarg2); 
    	dlog_int("DOWNLOAD RESULT", result);
 
		if(result){
			printf("[TEST CASE]: DOWNLOADED YOUTUBE VIDEO: %d\n", result); 
		}else{	
			printf("[TEST CASE]: FAIL: DOWNLOAD YOUTUBE VIDEO: %d\n", result); 
		}

	}
}


void runAllTests(char *myarg){

	
	if(strcmp(myarg, TEST) == 0){
		// run playlist tests
		testInsertPlaylist("t-cp", "testing-playlist"); 
		testLoadPlaylists("t-lp"); 
		testViewPlaylists("t-vp"); 
		testDeletePlaylist("t-dp", "testing-playlist");

	 
		// run song tests
		testInsertSong("t-cs", "testing-song"); 
		testLoadSongs("t-ls"); 
		testViewSongs("t-vs"); 
		testDeleteSong("t-ds", "testing-song"); 
	}

}

