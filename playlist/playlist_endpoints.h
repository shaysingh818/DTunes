#ifndef PLAYLIST_ENDPOINTS_H
#define PLAYLIST_ENDPOINTS_H

#include "../endpoints/endpoints.h"

// playlist functions
void insertPlaylistCmd(endpoint_t *e, char* argv[]);
void viewPlaylistCmd(endpoint_t *e, char* argv[]);
void deletePlaylistCmd(endpoint_t *e, char* argv[]);
void deletePlaylistByNameCmd(endpoint_t *e, char* argv[]);
void deleteAllPlaylistsCmd(endpoint_t *e, char* argv[]);
void addSongPlaylistCmd(endpoint_t *e, char* argv[]);
void viewPlaylistSongsCmd(endpoint_t *e, char* argv[]);
void testLoadPlaylists(endpoint_t *e, char* argv[]); 
void testPlaylistLibrary(endpoint_t *e, char* argv[]); 
void playlistEndpoints(endpoint_t **head); 

#endif
