#include "song.h"
#include "song_endpoints.h"
#include "song_test.h"
#include "../db/db.h"

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
        int dbResult = updateSongByName(argv[3], argv[2]);
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
        int result = deleteSongByName(argv[2]);
        if(result){
            d_log("DTUNES", "Deleted song by name");
        }else{
            printf("Something went wrong: refer to unit tests\n");
        }
    }
}



void deleteSongByIdCmd(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = deleteSongById(argv[2]);
        if(result){
            d_log("DTUNES", "Deleted song by id");
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

void syncAudioFilesToDb(endpoint_t *e, char* argv[]){
    // delete all playlists
    if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = loadAudioFilesFromDirectory("../../data/audiofiles");
        if(result){
            d_log("SYNC SERVICE", "Synced songs");
        }else{
            d_log("ERROR", "Failed to sync songs");
        }
    }

}


void testAudioFileLibrary(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
        runAudioFilesTest();
    }
}

void songEndpoints(endpoint_t **head){
	
	// delete songs
	endpoint_t *e5 = createEndpoint(
		"delete-songs",
		"das",
		"delete all songs in database"
    );
    e5->endpointLogic = deleteAllSongsCmd;
    appendEndpoint(head, e5);
	

	// delete song
	endpoint_t *e4 = createEndpoint(
		"delete-song",
		"ds",
		"delete song by name"
	);
    e4->endpointLogic = deleteSongCmd;
    appendEndpoint(head, e4);


	// update song
	endpoint_t *e3 = createEndpoint(
		"update-song",
		"us",
		"update song names on dtunes"
	);
	
	e3->endpointLogic = updateSongCmd; 
    appendEndpoint(head, e3);
	
	// view songs
	endpoint_t *e2 = createEndpoint(
		"view-songs",
		"vs",
		"view all songs on dtunes"
   	);
    e2->endpointLogic = viewSongsCmd;
    appendEndpoint(head, e2);

	
	// create song
	endpoint_t *e1 = createEndpoint(
		"create-song",
		"cs",
		"create song on dtunes using youtube url"
    );
    e1->endpointLogic = insertSongCmd;
    appendEndpoint(head, e1);

	
	// test audio library
	endpoint_t *e0 = createEndpoint(
		"test-audio-files",
		"ts",
		"test songs library"
    );
    e0->endpointLogic = testAudioFileLibrary;
    appendEndpoint(head, e0);

}
