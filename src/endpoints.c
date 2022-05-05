#include "endpoints.h"
#include "playlist.h"
#include "song.h"
#include "urls.h"
#include "db.h"
#include "test.h"


endpoint_t *createEndpoint(char *name, char *commandLineArg, char *docString){
    // create endpoint
    endpoint_t *newEndpoint = (endpoint_t*)malloc(sizeof(endpoint_t));
    strcpy(newEndpoint->name, name);
    strcpy(newEndpoint->commandLineArg, commandLineArg);
    newEndpoint->documentation = (char*)malloc(strlen(docString) + 1);
    strcpy(newEndpoint->documentation, docString);
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
        // go to next head reference
        head_ref = head_ref->next;
    }
}



void printEndpoints(endpoint_t *head_ref){
    // display endpoints
    char *column1 = "FLAG-COMMAND";
    char *column3 = "DOCUMENTATION";
    // display 
    printf("\e[0;31m");
    printf("%c[1m",27);
    printf("%-25s  %-25s\n", column1, column3);
	generateBanner(65); 
    while(head_ref != NULL){
        	printf("--%-20s  %-25s\n",
            head_ref->commandLineArg,
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



// view playlist
void viewPlaylistCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = viewPlaylists();  
		printf("\n"); 	
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


void addSongPlaylistCmd(endpoint_t *e, char* argv[]){
	if (strcmp(argv[1], e->commandLineArg) == 0){
		dlog("TESTING", "Playlist endpoint add song"); 
		if(argv[2] && argv[3]){
			int result = addSongToPlaylist(argv[2], argv[3]); 
			if(result){
				dlog("DB RELATION", "Added song to playlist"); 
			}else{
				dlog("DB RELATION FAILED", "Could not add song to playlist"); 
			}
		}
	}
}



void viewPlaylistSongsCmd(endpoint_t *e, char* argv[]){
	if (strcmp(argv[1], e->commandLineArg) == 0){
		viewPlaylistSongs(argv[2]);  
	}
}


void testLoadPlaylists(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
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

		// change to desired directory
    	if(chdir("../../src") != 0){ 
    	  dlog("ERROR", "CHANGE DIR FOR DB SYNC"); 
    	}


		// sync songs after download	
		result = loadAudioFilesFromDirectory("../data/audiofiles");
		if(result){
			d_log("SYNC SERVICE", "Synced songs"); 	
		}else{
			d_log("ERROR", "Failed to sync songs"); 
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



void viewSongsCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){	
		viewSongs(); 
		printf("\n"); 	
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


void testLoadSongs(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
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



void viewYoutubeUrlsCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){	
		int result = viewUrls(); 
		printf("\n"); 	
	}
}




void deleteYoutubeUrlCmd(endpoint_t *e, char* argv[]){	
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = deleteYoutubeUrl(argv[2]); 	
		if(result){
			d_log("DTUNES", "Deleted youtube url"); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
		}
	}
}



void deleteAllYoutubeUrlsCmd(endpoint_t *e, char* argv[]){
	// delete all playlists
	if (strcmp(argv[1], e->commandLineArg) == 0){
		int result = deleteAllYoutubeUrls();
		if(result){	
			d_log("DTUNES", "Deleted all urls"); 
		}else{
			printf("Something went wrong: refer to unit tests\n"); 
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



void YTBackup(endpoint_t *e, char* argv[]){
    // test view songs  
	if (strcmp(argv[1], e->commandLineArg) == 0){
		if(argv[2]){
			int threadCount; 
			sscanf(argv[2], "%d", &threadCount);   
			grabDatabaseUrls(threadCount);  
		}else{
			grabDatabaseUrls(2); // default 2 threads
		}
    }
}


void runUnitTests(endpoint_t *e, char* argv[]){
    // test view songs  
	if (strcmp(argv[1], e->commandLineArg) == 0){
		runAllTests(); // run all the tests in the unit library
    }
}



