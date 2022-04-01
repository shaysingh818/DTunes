#include "endpoints.h"
#include "playlist.h"
#include "song.h"
#include "db.h"


endpoint_t *createEndpoint(char *name, char *commandLineArg, char *docString){
    // create endpoint
    endpoint_t *newEndpoint = (endpoint_t*)malloc(sizeof(endpoint_t));
    strcpy(newEndpoint->name, name);
    strcpy(newEndpoint->commandLineArg, commandLineArg);
    newEndpoint->documentation = (char*)malloc(strlen(docString) + 1);
    strcpy(newEndpoint->documentation, docString);

	char testCommand[256];
    strcpy(testCommand, "t-");
    strcat(testCommand, newEndpoint->commandLineArg);
    strcpy(newEndpoint->testCommandLineArg, testCommand);
    return newEndpoint;
} 


void appendEndpoint(endpoint_t **head_ref, endpoint_t *e1){
    // add endpoint in linked list
    e1->next = (*head_ref);
    (*head_ref) = e1;
}


void execEndpoints(int argc, char *argv[], endpoint_t *head_ref){
    while(head_ref != NULL){	
		 // check arg condition
        head_ref->endpointLogic(head_ref, argv);
        head_ref->testEndpointLogic(head_ref, argv);
        // go to next head reference
        head_ref = head_ref->next;
    }
}



void printEndpoints(endpoint_t *head_ref){
    // display endpoints
    char *column1 = "FLAG-COMMAND";
    char *column2 = "TESTING";
    char *column3 = "DOCUMENTATION";
    // display 
    printf("\e[0;31m");
    printf("%c[1m",27);
    printf("%-17s  %-17s  %-30s\n", column1, column2, column3);
    printf("==========================================================\n");
    while(head_ref != NULL){
        printf("--%-15s  %-15s  %-25s\n",
            head_ref->commandLineArg,
            head_ref->testCommandLineArg,
            head_ref->documentation
        );
        head_ref = head_ref->next;
    }
}




// create playlist
void insertPlaylistCmd(endpoint_t *e, char* argv[]){
	if(strcmp(argv[1], e->commandLineArg) == 0){
		// bind fields for song model
		char *mytime = getCurrentTime(); 
		// create instance of playlist model	
		playlist_t *newPlaylist;
		newPlaylist = (playlist_t*)malloc(sizeof(playlist_t));	
		printf("\033[0;32m");
		printf("[%s]: CREATED INSTANCE OF PLAYLIST:\n", "STRUCTURE ALLOCATION"); 
		// set values
		strcpy(newPlaylist->name, argv[2]);	
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


// test create playlist
void testInsertPlaylist(endpoint_t *e,char* argv[]){	
	if(strcmp(argv[1], e->testCommandLineArg) == 0){
    
        // bind fields for song model
        char *mytime = getCurrentTime(); 
        // create instance of playlist model    
        playlist_t *newPlaylist;
        newPlaylist = (playlist_t*)malloc(sizeof(playlist_t));  
        printf("\033[0;32m\n");
        printf("[%s]: CREATED INSTANCE OF PLAYLIST:\n", "STRUCTURE ALLOCATION"); 

        // set values
        strcpy(newPlaylist->name, argv[2]);    
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




// view playlist
void viewPlaylistCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = viewPlaylists();  
		printf("\n"); 	
	}
}


// test view playlists
void testViewPlaylists(endpoint_t *e, char* argv[]){
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
        printf("View playlists test case: \n");
        int result = viewPlaylists();
    }
}



void deletePlaylistCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = deletePlaylist(argv[2]); 	
		if(result){
			printf("[DTUNES]: Deleted Playlist: %s\n", argv[2]); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}
}

void testDeletePlaylist(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
        // test function
        int result = deletePlaylist(argv[2]);
        if(result){
            printf("PASS: Deleted Playlist: %d\n", result);
        }else{
            printf("FAIL: Deleted Playlist: %d\n", result);
        }
    }
}



void deleteAllPlaylistsCmd(endpoint_t *e, char* argv[]){
	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = deleteAllPlaylists();
		if(result){
			printf("[DTUNES]: Deleted Playlists\n");
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}
}


void testDeletePlaylists(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
        int result = deleteAllPlaylists();
        if(result){
            printf("[TEST CASE]: PASS Deleted ALL playlists: %d\n", result);
        }else{
            printf("[TEST CASE] FAIL Deleted ALL playlists: %d\n", result);
        }
    }
}


void testLoadPlaylists(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
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


void insertSongCmd(endpoint_t *e, char* argv[]){
	// insert song into db	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		// bind fields for song model
		int result = downloadVideo(argv[2]);
        dlog_int("DOWNLOAD RESULT", result);

        if(result){
            printf("[TEST CASE]: DOWNLOADED YOUTUBE VIDEO: %d\n", result);
        }else{
            printf("[TEST CASE]: FAIL: DOWNLOAD YOUTUBE VIDEO: %d\n", result);
        }
	}

}

void testInsertSong(endpoint_t *e, char* argv[]){
  
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
        // bind fields for song model
        char *mytime = getCurrentTime(); 
        char *songName = argv[2]; 
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

void updateSongCmd(endpoint_t *e, char *argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
        // insert song model struct into db
        int dbResult = updateSong(argv[3], argv[2]);
        // check if database insert was successful 
        if(dbResult){
            printf("\033[0;32m");
            printf("[TEST CASE]: UPDATE SONG: passed \n");
        }else{
            printf("\e[0;31m");
            printf("[TEST CASE]: UPDATE SONG: failed\n");
        }
    }
}



void testUpdateSong(endpoint_t *e, char* argv[]){
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
        // insert song model struct into db
        int dbResult = updateSong("test-song", argv[2]);
        // check if database insert was successful 
        if(dbResult){
            printf("\033[0;32m");
            printf("[TEST CASE]: UPDATE SONG: passed \n");
        }else{
            printf("\e[0;31m");
            printf("[TEST CASE]: UPDATE SONG: failed\n");
        }
    }
}



void viewSongsCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){	
		viewSongs(); 
		printf("\n"); 	
	}
}


void testViewSongs(endpoint_t *e, char* argv[]){
    // test view songs  
	if (strcmp(argv[1], e->testCommandLineArg) == 0){	
        int result = viewSongs();
    }

}



void deleteSongCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = deleteSong(argv[2]); 	
		if(result){
			d_log("DTUNES", "Deleted song"); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}
}


void testDeleteSong(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
        int result = deleteSong(argv[2]);
        if(result){
            printf("[TEST CASE]: PASS Deleted song: %d\n", result); 
        }else{  
            printf("[TEST CASE]: FAIL: Deleted songs: %d\n", result); 
        }
    }
}



void deleteAllSongsCmd(endpoint_t *e, char* argv[]){
	// delete all playlists
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = deleteAllSongs();
		if(result){	
			d_log("DTUNES", "Deleted all songs"); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}

}


void testDeleteSongs(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
        int result = deleteAllSongs();
        if(result){
            printf("[TEST CASE]: PASS Deleted ALL songs: %d\n", result);
        }else{
            printf("[TEST CASE]: FAIL: Deleted ALL playlists: %d\n", result);
        }
    }
}


void testLoadSongs(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->testCommandLineArg) == 0){
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


void insertYoutubeUrlCmd(endpoint_t *e, char* argv[]){
	// insert song into db	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		char *currTime = getCurrentTime();
		// bind fields for song model
		int result = insertUrl(argv[2], currTime);
        if(result){
            printf("INSERTED YOUTUBE URL: %d\n", result);
        }else{
            printf("FAIL: COULD NOT YOUTUBE URL: %d\n", result);
        }
	}

}



void testInsertYoutubeUrlCmd(endpoint_t *e, char* argv[]){
	// insert song into db	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		char *currTime = getCurrentTime();
		// bind fields for song model
		int result = insertUrl(argv[2], currTime);
        if(result){
            printf("[PASS]: INSERTED YOUTUBE URL: %d\n", result);
        }else{
            printf("[FAIL TEST CASE]: COULD NOT YOUTUBE URL: %d\n", result);
        }
	}

}


void viewYoutubeUrlsCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){	
		int result = viewUrls(); 
		printf("\n"); 	
	}
}



void testViewYoutubeUrlsCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){	
		int result = viewUrls(); 
		printf("\n"); 	
		if(result){
			d_log("SYNC SERVICE", "Synced songs"); 	
		}else{
			d_log("ERROR", "Failed to sync songs"); 
		}
	}
}


void syncAudioFilesToDb(endpoint_t *e, char* argv[]){
	// delete all playlists
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = loadAudioFilesFromDirectory("../data/audiofiles");
		if(result){
			d_log("SYNC SERVICE", "Synced songs"); 	
		}else{
			d_log("ERROR", "Failed to sync songs"); 
		}
	}

}



void testSyncAudioFilesToDb(endpoint_t *e, char* argv[]){
	// delete all playlists
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = loadAudioFilesFromDirectory("../data/audiofiles");
		if(result){
			d_log("TEST CASE[SYNC_SONG]", "PASSED"); 	
		}else{
			d_log("TEST CASE[SYNC_SONG]", "FAILED"); 
		}
	}
}


