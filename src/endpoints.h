#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>


struct Endpoint {
    char name[100];
    char commandLineArg[100];
    char *documentation;
    void (*endpointLogic)(struct Endpoint* e, char* argv[]);
    struct Endpoint* next;
};

typedef struct Endpoint endpoint_t;


// endpoint library functions
endpoint_t *createEndpoint(char *name, char *commandLineArg, char *testCommand);
void appendEndpoint(endpoint_t **head_ref, endpoint_t *e1); 
void execEndpoints(int argc, char *argv[], endpoint_t *head_ref);
void printEndpoints(endpoint_t *head_ref);

// playlist functions
void insertPlaylistCmd(endpoint_t *e, char* argv[]);
void viewPlaylistCmd(endpoint_t *e, char* argv[]); 
void deletePlaylistCmd(endpoint_t *e, char* argv[]); 
void deleteAllPlaylistsCmd(endpoint_t *e, char* argv[]); 
void addSongPlaylistCmd(endpoint_t *e, char* argv[]); 
void viewPlaylistSongsCmd(endpoint_t *e, char* argv[]); 

// in memory playlist operation
void testLoadPlaylists(struct Endpoint *e, char* argv[]);

// song functions
void insertSongCmd(endpoint_t *e, char* argv[]); 
void updateSongCmd(endpoint_t *e, char* argv[]); 
void viewSongsCmd(endpoint_t *e, char* argv[]);
void deleteSongCmd(endpoint_t *e, char* argv[]);
void deleteAllSongsCmd(endpoint_t *e, char* argv[]); 
 
// in memory song operation
void testLoadSongs(endpoint_t *e, char* argv[]);

// youtube urls storage
void insertYoutubeUrlCmd(endpoint_t *e, char* argv[]); 
void viewYoutubeUrlsCmd(endpoint_t *e, char* argv[]);
void deleteYoutubeUrlCmd(endpoint_t *e, char* argv[]); 
void deleteAllYoutubeUrlsCmd(endpoint_t *e, char* argv[]); 

// syncing
void syncAudioFilesToDb(endpoint_t *e, char* argv[]); 

// youtube url backup
void YTBackup(endpoint_t *e, char* argv[]); 

 #endif 
