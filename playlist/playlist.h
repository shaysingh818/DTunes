#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string.h>
#include <uuid/uuid.h> 
#include "../audiofile/song.h"

/**
	Model for storing songs/audio files in container
*/
struct AudioPlaylist {
	uuid_t playlistId; 
	char name[100]; 
	char dateCreated[30]; 
};

typedef struct AudioPlaylist playlist_t;  


// functions
playlist_t **initPlaylists(int limit);
playlist_t *viewPlaylistById(char *uuid);  
playlist_t *viewPlaylistByName(char *playlistName);  
int retrieveLastPlaylistId();
int getPlaylistTableSize(); 

// crud operations
int createPlaylist(playlist_t* playlist);
int viewPlaylists(); 
int deletePlaylist(char *playlistUuid);
int deletePlaylistByName(char *playlistName);  
int deleteCascadingPlaylists(char *playlistUuid); 
int deleteAllPlaylists(); 
int deleteAllPlaylistRelations();  
int checkPlaylistExist(char *playlistName);
int addSongToPlaylist(char *songUuid, char *playlistUuid); 
int viewSongsPlaylist(char *playlistUuid); 
int getRelationTableSize(char *playlistUuid); 
song_t **loadPlaylistSongs(char *playlistUuid); 
void viewPlaylistSongs(char *playlistUuid); 

#endif 
