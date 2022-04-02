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


void urlEndpoints(){

	
	// view youtube urls
	endpoint_t *e11 = createEndpoint(
                        "view-urls",
                        "vu",
                        "view all youtube urls on dtunes"
                    );
    e11->endpointLogic = viewYoutubeUrlsCmd;
    e11->testEndpointLogic = testViewYoutubeUrlsCmd;
    appendEndpoint(&head, e11);

	
	// create youtube url
	endpoint_t *e12 = createEndpoint(
                        "create-url",
                        "cu",
                        "insert youtube url"
                    );
    e12->endpointLogic = insertYoutubeUrlCmd;
    e12->testEndpointLogic = testInsertYoutubeUrlCmd;
    appendEndpoint(&head, e12);


	// delete urls
	endpoint_t *e13 = createEndpoint(
                        "delete-urls",
                        "dus",
                        "delete all urls in database"
                    );
    e13->endpointLogic = deleteAllYoutubeUrlsCmd;
    e13->testEndpointLogic = testDeleteYoutubeUrls; 
    appendEndpoint(&head, e13);
	
	// delete url
	endpoint_t *e14 = createEndpoint(
                        "delete-url",
                        "du",
                        "delete youtube url by url link"
                    );
    e14->endpointLogic = deleteYoutubeUrlCmd;
    e14->testEndpointLogic = testDeleteYoutubeUrl;
    appendEndpoint(&head, e14);



	// create youtube url download backup process
	endpoint_t *e15 = createEndpoint(
                        "yt-backup",
                        "yb",
                        "download youtube urls"
                    );
    e15->endpointLogic = YTBackup;
    e15->testEndpointLogic = testYTBackup;
    appendEndpoint(&head, e15);

	

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
	urlEndpoints();  

	if(strcmp(argv[1], "help") == 0){
		printEndpoints(head); 
	}

	execEndpoints(argc, argv, head); 

	


	return 0; 
}	
