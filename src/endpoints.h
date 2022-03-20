#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>

#define HELP_MENU  "help"

// playlists
#define INSERT_PLAYLIST  "cp"
#define VIEW_PLAYLIST  "vp"
#define DELETE_PLAYLIST  "dp" 
#define DELETE_ALL_PLAYLIST  "dps"

//songs
#define INSERT_SONG  "cs"
#define VIEW_SONG  "vs"
#define DELETE_SONG  "ds" 
#define DELETE_ALL_SONG  "dss"

struct Endpoint {
    char name[100];
    char commandLineArg[100];
    char testCommandLineArg[100];
    char *documentation;
    // define endpoint patterns here
    void (*endpointLogic)(struct Endpoint* e,char *arg1, char *arg2);
    void (*endpointUpdateLogic)(struct Endpoint* e,char *arg1, char *arg2, char *arg3);     
    void (*testEndpointLogic)(struct Endpoint* e,char *arg1, char *arg2);
    struct Endpoint* next;
};

typedef struct Endpoint endpoint_t;


// endpoint library functions
endpoint_t *createEndpoint(char *name, char *commandLineArg, char *testCommand);
void appendEndpoint(endpoint_t **head_ref, endpoint_t *e1); 
void execEndpoints(char *argv[], endpoint_t *head_ref); 
void printEndpoints(endpoint_t *head_ref);

// playlist functions
void insertPlaylistCmd(struct Endpoint *e,char *myarg, char *myarg2);
void testInsertPlaylist(struct Endpoint *e, char *myarg, char *myarg2);
void viewPlaylistCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void testViewPlaylists(struct Endpoint *e, char *myarg, char *myarg2); 
void deletePlaylistCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void testDeletePlaylist(struct Endpoint *e, char *myarg, char *myarg2);
void deleteAllPlaylistsCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void testDeletePlaylists(struct Endpoint *e, char *myarg, char *myarg2);

// in memory playlist operation
void testLoadPlaylists(struct Endpoint *e, char *myarg, char *myarg2);

// song functions
void insertSongCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void testInsertSong(struct Endpoint *e, char *myarg, char *myarg2);
void updateSongCmd(struct Endpoint *e, char *myarg, char *myarg2, char *myarg3); 
void testUpdateSong(struct Endpoint *e, char *myarg, char *myarg2, char *myarg3);
void viewSongsCmd(struct Endpoint *e, char *myarg, char *myarg2);
void testViewSongs(struct Endpoint *e, char *myarg, char *myarg2);
void deleteSongCmd(struct Endpoint *e, char *myarg, char *myarg2);
void testDeleteSong(struct Endpoint *e, char *myarg, char *myarg2);
void deleteAllSongsCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void testDeleteSongs(struct Endpoint *e, char *myarg, char *myarg2);
 
// in memory song operation
void testLoadSongs(struct Endpoint *e, char *myarg, char *myarg2);

// syncing
void syncAudioFilesToDb(struct Endpoint *e, char *myarg, char *myarg2); 


 #endif 
