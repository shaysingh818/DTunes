#include "playlist_test.h"
#include "playlist.h"
#include "../db/db.h"

void testCreatePlaylistCollection(){
    
    char *mytime = getCurrentTime();
    // create playlist instance
    playlist_t *newPlaylist;
    newPlaylist = (playlist_t*)malloc(sizeof(playlist_t));
    strcpy(newPlaylist->name, "test-playlist");
    strcpy(newPlaylist->dateCreated, mytime);
    int insertDbResult = createPlaylist(newPlaylist);
    // check result of insert 
    if(insertDbResult){
        //printf("\033[0;32m")  
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "INSERT PLAYLIST COLLECTION");
    }else{
        printf("\e[0;31m"); 
        dlog("FAILED", "INSERT PLAYLIST COLLECTION");
    }
    
}  


void testDeletePlaylistCollection(){

    int dbResult = deletePlaylistByName("test-playlist");
    if(dbResult){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE PLAYLIST COLLECTION");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE PLAYLIST COLLECTION");
    }
}


void testDeleteAllPlaylistCollections(){

    int dbResult = deleteAllPlaylists();
    if(!dbResult){
        printf("\033[0;31"); // red for fail
        dlog("FAILED", "DELETE ALL PLAYLISTS");
    }

    int playlistResult = getPlaylistTableSize();
    if(playlistResult == 0){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE ALL PLAYLISTS");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE ALL PLAYLISTS");
    }
}




void testLoadPlaylistsArray(){

    char *mytime = getCurrentTime();
    // allocate space for three playlists
    playlist_t **playlists = malloc(3 * sizeof(playlist_t*));
    for(int i = 0; i < 3; i++){
        playlists[i] = malloc(sizeof(playlist_t));
    }

    strcpy(playlists[0]->name, "testing 1");
    strcpy(playlists[0]->dateCreated, mytime);

    strcpy(playlists[1]->name, "testing 2");
    strcpy(playlists[1]->dateCreated, mytime);

    strcpy(playlists[2]->name, "testing 3");
    strcpy(playlists[2]->dateCreated, mytime);

    // loop and insert all of them
    for(int j = 0; j < 3; j++){
        int insertDbResult = createPlaylist(playlists[j]);
        // check result of insert 
        if(!insertDbResult){
            printf("\033[0;31m"); // green for pass
            dlog("FAILED", "LOAD PLAYLIST TEST CASE");
        }
    }

    // testing what's in the database
    int playlistLimit = getPlaylistTableSize();
    playlist_t **playlistsDB = initPlaylists(playlistLimit);
    playlist_t ***p = &playlistsDB;

    for(int i = 0; i < playlistLimit; i++){
        int checkName = strcmp(playlists[i]->name, (*p)[i]->name);
        int checkDate = strcmp(playlists[i]->dateCreated, (*p)[i]->dateCreated);

        if(checkName == 0 && checkDate == 0){
            printf("\033[0;32m"); // green for pass
            dlog("PASSED", "LOAD PLAYLIST");
        }else{
            printf("\033[0;31m"); // red for fail
            dlog("FAILED", "LOAD PLAYLIST");
        }

    }

    int dpResult = deleteAllPlaylists();
    if(!dpResult){
        dlog("FAILED", "DELETE PLAYLIST LOADED");
    }

}



void testAddSongToPlaylist(){

	// create playlist
	char *mytime = getCurrentTime(); 	
	playlist_t *newPlaylist; 
	newPlaylist = (playlist_t*)malloc(sizeof(playlist_t)); 
	strcpy(newPlaylist->name, "test-playlist"); 
	strcpy(newPlaylist->dateCreated, mytime); 
	int insertDbResult = createPlaylist(newPlaylist);
    // check result of insert 
    if(!insertDbResult){
        printf("\e[0;31m");	
		dlog("FAILED", "ADD SONG TO PLAYLIST"); 
    }

	// allocate space for three playlists
	song_t **songs = malloc(3 * sizeof(song_t*));
	for(int i = 0; i < 3; i++){
		songs[i] = malloc(sizeof(song_t));  
	}

	strcpy(songs[0]->name, "test song 1 (add to playlist) "); 	
	strcpy(songs[0]->dateCreated, mytime);	
	strcpy(songs[0]->filePath, "test file path 1");
	strcpy(songs[0]->subtitles, "test subtitles 1");
	 
	strcpy(songs[1]->name, "test song 2 (add to playlist) "); 	
	strcpy(songs[1]->dateCreated, mytime); 	
	strcpy(songs[1]->filePath, "test file path 2");
	strcpy(songs[1]->subtitles, "test subtitles 2");
		
	strcpy(songs[2]->name, "test song 3 (add to playlist) "); 	
	strcpy(songs[2]->dateCreated, mytime); 
	strcpy(songs[2]->filePath, "test file path 3");
	strcpy(songs[2]->subtitles, "test subtitles 3");

	// insert 3 songs
	for(int j = 0; j < 3; j++){
		int insertDbResult = createSong(songs[j]);
    	// check result of insert 
    	if(!insertDbResult){
			printf("\033[0;31m"); // green for pass
			dlog("FAILED", "ADD SONG TO PLAYLIST");
		} 
	}
	
	// get songs from the database
	int songLimit = getSongTableSize(); 
	song_t **songsDB = initSongs(songLimit); 
	song_t ***p = &songsDB;

	// view playlist
	playlist_t *playlist = viewPlaylistByName("test-playlist");  
	char playlist_uuid[37]; 
	uuid_unparse_lower(playlist->playlistId, playlist_uuid); 


	// add every song to the playlist
	for(int i = 0; i < songLimit; i++){
		char song_uuid[37]; 
		uuid_unparse_lower((*p)[i]->songId, song_uuid); 
		// insert song to playlist
		int addSong = addSongToPlaylist(playlist_uuid, song_uuid); 
		if(!addSong){
			dlog("FAILED", "ADD SONG TO PLAYLIST"); 
		}
    }


	// test if songs in playlist are similar to insert
	int relationLimit = getRelationTableSize(playlist_uuid);
    song_t **playlistSongs = loadPlaylistSongs(playlist_uuid);
    song_t ***pSongs = &playlistSongs;

	
	for(int j = 0; j < relationLimit; j++){
		int checkName = strcmp(playlistSongs[j]->name, (*pSongs)[j]->name);	
		int checkDate = strcmp(playlistSongs[j]->dateCreated, (*pSongs)[j]->dateCreated);	
		int checkPath = strcmp(playlistSongs[j]->filePath, (*pSongs)[j]->filePath);	
		int checkSubtitles = strcmp(songs[j]->subtitles, (*pSongs)[j]->subtitles);

		int firstCondition = checkName == 0 && checkDate == 0; 
		int secondCondition = checkPath == 0 && checkSubtitles == 0; 

		if(firstCondition && secondCondition){			
			printf("\033[0;32m"); // green for pass
			dlog("PASSED", "ADD SONG TO PLAYLIST"); 
		}else{	
			printf("\033[0;31m"); // red for fail
			dlog("FAILED", "ADD SONG TO PLAYLIST"); 
		}

	}

	
	int deleteSongs = deleteAllSongs(); 
	int deletePlaylists = deleteAllPlaylists(); 
	if(!deleteSongs){
		dlog("FAILED", "DELETE ALL SONGS (ADD TO PLAYLIST)"); 
	}

		
	if(!deletePlaylists){
		dlog("FAILED", "DELETE ALL PLAYLISTS (ADD TO PLAYLIST)"); 
	}	

	
}



void runPlaylistTests(){
	printf("\033[0;37m");
    dlog("PLAYLIST LIBRARY", "Running tests for playlist library");
    testCreatePlaylistCollection();
    testDeletePlaylistCollection();
    testDeleteAllPlaylistCollections();
    testLoadPlaylistsArray();
    testAddSongToPlaylist();
}

