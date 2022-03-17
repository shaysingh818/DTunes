#include "endpoints.h"
#include "playlist.h"
#include "song.h"
#include "db.h"


// function create an endpoint
endpoint_t *createEndpoint(char *name, char *commandLineArg){
    endpoint_t *e1;
    e1 = (endpoint_t*)malloc(sizeof(endpoint_t));
    // bind values
    strcpy(e1->name, name);
    strcpy(e1->commandLineArg, commandLineArg);
    return e1;  
} 


void setDocumentationString(struct Endpoint* e, char *docBuffer){
    e->documentation = (char*)malloc(strlen(docBuffer) + 1);
    strcpy(e->documentation, "testing");
    printf("Doc buffer set\n"); 
}


void printDTunesBanner(FILE *fptr){
    char read_string[128];
    while(fgets(read_string,sizeof(read_string),fptr) != NULL){
        printf("%s",read_string);
	}
}

void DTunesHelpMenu(struct Endpoint* e, char *myarg, char *myarg2){
	
	char *column1 = "FLAG-COMMAND"; 
	char *column2 = "DOCUMENTATION"; 

	if(strcmp(myarg, e->commandLineArg) == 0){
		// help menu for playlists

		// display banner
		char *filename = "banner.txt";
    	FILE *fptr = NULL;
	
    	if((fptr = fopen(filename,"r")) == NULL){
        	fprintf(stderr,"error opening %s\n",filename);
    	}
    	printDTunesBanner(fptr);
		printf("DTUNES testing suite for all methods and features that are being added.: \n\n"); 
    	fclose(fptr);


		// set color	
		printf("\e[0;31m");	
		printf("%c[1m",27);
		printf("%-25s  %-30s\n", column1, column2);
		printf("==========================================================\n"); 
		// help menu for songs
		printf("--%-25s:  create a playlist\n", INSERT_PLAYLIST); 
		printf("--%-25s:  view playlists in database \n", VIEW_PLAYLIST); 
		printf("--%-25s:  delete playlist by name \n", DELETE_PLAYLIST); 
		printf("--%-25s:  delete all playlists in database \n", DELETE_ALL_PLAYLIST); 

		// help menu for songs
		printf("--%-25s:  insert song using youtube url\n", INSERT_SONG); 
		printf("--%-25s:  view songs in database\n", VIEW_SONG); 	
		printf("--%-25s:  delete song by name \n", DELETE_SONG); 
		printf("--%-25s:  delete all songs in database \n", DELETE_ALL_SONG);
		printf("\n");  
	}
}

 
void insertPlaylistCmd(struct Endpoint *e, char *myarg, char *myarg2){
	if(strcmp(myarg, e->commandLineArg) == 0){
		// bind fields for song model
		char *playlistName = myarg2; 	
		char *mytime = getCurrentTime(); 

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
	
	}
}


void viewPlaylistCmd(struct Endpoint *e, char *myarg, char *myarg2){	
	if (strcmp(myarg, e->commandLineArg) == 0){
		int result = viewPlaylists();  
		printf("\n"); 	
	}
}


void deletePlaylistCmd(struct Endpoint *e, char *myarg, char *myarg2){
	
	if (strcmp(myarg, e->commandLineArg) == 0){
		char *playlistName = myarg2; 
		int result = deletePlaylist(playlistName); 	
		if(result){
			printf("[DTUNES]: Deleted Playlist: %s\n", playlistName); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}
}


void deleteAllPlaylistsCmd(struct Endpoint *e, char *myarg, char *myarg2){
	
	if (strcmp(myarg, e->commandLineArg) == 0){
		int result = deleteAllPlaylists();
		if(result){
			printf("[DTUNES]: Deleted Playlists\n");
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}

}


void insertSongCmd(char *myarg, char *myarg2){
		// insert song into db
	if(strcmp(myarg, INSERT_SONG) == 0){	
		// bind fields for song model

		int result = downloadVideo(myarg2);
        dlog_int("DOWNLOAD RESULT", result);

        if(result){
            printf("[TEST CASE]: DOWNLOADED YOUTUBE VIDEO: %d\n", result);
        }else{
            printf("[TEST CASE]: FAIL: DOWNLOAD YOUTUBE VIDEO: %d\n", result);
        }


		/**
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

		
		// insert song model struct into db
		int dbResult = createSong(newSong);

		// check if database insert was successful 
		if(dbResult){
			printf("\033[0;32m");
			d_log("DTUNES", "INSERTED SONG");
		}else{	
			printf("Something went wrong: refer to unit tests\n"); 

		}
		*/
	}

}


void viewSongsCmd(char *myarg){	
	if (strcmp(myarg, VIEW_SONG) == 0){
		viewSongs(); 
		printf("\n"); 	
	}
}



void deleteSongCmd(char *myarg, char *myarg2){
	if(strcmp(myarg, DELETE_SONG) == 0){
		char *songName = myarg2; 
		int result = deleteSong(songName); 	
		if(result){
			d_log("DTUNES", "Deleted song"); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}
}


void deleteAllSongsCmd(char *myarg){
	// delete all playlists
	if(strcmp(myarg, DELETE_ALL_SONG) == 0){	
		int result = deleteAllSongs();
		if(result){	
			d_log("DTUNES", "Deleted all songs"); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}

}




