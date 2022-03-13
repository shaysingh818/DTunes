#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string.h>


/**
	Model for storing songs/audio files in container
*/
struct AudioPlaylist {
	char name[100]; 
	char dateCreated[30]; 
};

typedef struct AudioPlaylist playlist_t;  


// functions
playlist_t **initPlaylists(int limit);
int retrieveLastPlaylistId();
int getPlaylistTableSize(); 

// crud operations
int createPlaylist(playlist_t* playlist);
int viewPlaylists(); 
int deletePlaylist(char *playlistName); 
int deleteAllPlaylists();  
int checkPlaylistExist(char *playlistName);

#endif 
