#include "playlist.h"
#include "playlist_endpoints.h"
#include "playlist_test.h"
#include "../db/db.h"

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


void deletePlaylistByNameCmd(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
        int result = deletePlaylistByName(argv[2]);
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


void testPlaylistLibrary(endpoint_t *e, char* argv[]){
    if (strcmp(argv[1], e->commandLineArg) == 0){
		runPlaylistTests(); 
    }
}


void playlistEndpoints(endpoint_t **head){

	// delete songs
	endpoint_t *e7 = createEndpoint(
    	"delete-playlists",
        "dap",
        "delete all playlists in database"
    );
    e7->endpointLogic = deleteAllPlaylistsCmd;
    appendEndpoint(head, e7);

	
	// delete song
	endpoint_t *e8 = createEndpoint(
    	"delete-playlist",
        "dp",
        "delete playlist by uuid"
    );
    e8->endpointLogic = deletePlaylistCmd;
    appendEndpoint(head, e8);

	
	// delete song
	endpoint_t *e85 = createEndpoint(
    	"delete-playlist-by-name",
        "dpn",
        "delete playlist by name"
    );
    e85->endpointLogic = deletePlaylistByNameCmd;
    appendEndpoint(head, e85);

	
	// view songs
	endpoint_t *e9 = createEndpoint(
    	"view-playlists",
    	"vp",
    	"view all playlists on dtunes"
    );
    e9->endpointLogic = viewPlaylistCmd;
    appendEndpoint(head, e9);

	
	// create song
	endpoint_t *e10 = createEndpoint(
    	"create-playlist",
    	"cp",
    	"create playlist on dtunes"
    );
    e10->endpointLogic = insertPlaylistCmd;
    appendEndpoint(head, e10);

	
	// create song
	endpoint_t *e11 = createEndpoint(
    	"add-song-playlist",
        "asp",
        "add song to playlist"
    );
    e11->endpointLogic = addSongPlaylistCmd;
    appendEndpoint(head, e11);

	
	// create song
	endpoint_t *e12 = createEndpoint(
		"view-songs-playlist",
        "vsp",
        "view songs in a playlist"
    );
    e12->endpointLogic = viewPlaylistSongsCmd;
    appendEndpoint(head, e12);


	// test playlist library
	endpoint_t *e13 = createEndpoint(
		"test-playlist-library",
        "tp",
        "test playlist library functions"
    );
    e13->endpointLogic = testPlaylistLibrary;
    appendEndpoint(head, e13);

}

