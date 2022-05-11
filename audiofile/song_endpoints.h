#ifndef SONG_ENDPOINTS_H
#define SONG_ENDPOINTS_H

#include "../endpoints/endpoints.h"

// song functions
void insertSongCmd(endpoint_t *e, char* argv[]);
void updateSongCmd(endpoint_t *e, char* argv[]);
void viewSongsCmd(endpoint_t *e, char* argv[]);
void deleteSongCmd(endpoint_t *e, char* argv[]);
void deleteAllSongsCmd(endpoint_t *e, char* argv[]);
void testLoadSongs(endpoint_t *e, char* argv[]); 
void syncAudioFilesToDb(endpoint_t *e, char* argv[]);
void testAudioFileLibrary(endpoint_t *e, char* argv[]);
void songEndpoints(endpoint_t **head); 


#endif

