#include "urls_endpoints.h"
#include "urls_test.h" 
#include "urls.h"
#include "../audiofile/song.h"
#include "../db/db.h"



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

		 char cwd[256];
    	if(getcwd(cwd, sizeof(cwd)) == NULL){
        	dlog("ERROR", "CURRENT WORKING DIRECTORY");
    	}


		/**
		int result = loadAudioFilesFromDirectory("data/audiofiles");
        if(result){
            d_log("SYNC SERVICE", "Synced songs");
        }else{
            d_log("ERROR", "Failed to sync songs");
        }
		*/

    }
}


void testUrlsLibrary(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
        runUrlsTest();
    }
}


void urlEndpoints(endpoint_t **head){
	
	// view youtube urls
	endpoint_t *e11 = createEndpoint(
		"view-urls",
		"vu",
		"view all youtube urls on dtunes"
    );
    e11->endpointLogic = viewYoutubeUrlsCmd;
    appendEndpoint(head, e11);

	
	// create youtube url
	endpoint_t *e12 = createEndpoint(
		"create-url",
		"cu",
		"insert youtube url"
    );
    e12->endpointLogic = insertYoutubeUrlCmd;
    appendEndpoint(head, e12);


	// delete urls
	endpoint_t *e13 = createEndpoint(
		"delete-urls",
		"dus",
		"delete all urls in database"
    );
    e13->endpointLogic = deleteAllYoutubeUrlsCmd;
    appendEndpoint(head, e13);
	
	// delete url
	endpoint_t *e14 = createEndpoint(
		"delete-url",
		"du",
		"delete youtube url by url link"
    );
    e14->endpointLogic = deleteYoutubeUrlCmd;
    appendEndpoint(head, e14);



	// create youtube url download backup process
	endpoint_t *e15 = createEndpoint(
    	"yt-backup",
        "yb",
        "download youtube urls"
    );
    e15->endpointLogic = YTBackup;
    appendEndpoint(head, e15);


	// add unit tests down here
	endpoint_t *e16 = createEndpoint(
		"urls-test",
		"tu",
		"run all unit tests for mp3 dataset library"
    );
    e16->endpointLogic = testUrlsLibrary;
    appendEndpoint(head, e16);
}

