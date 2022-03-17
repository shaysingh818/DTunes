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
    char *documentation;
    void (*endpointLogic)(struct Endpoint* e,char *arg1, char *arg2);
    void (*testEndpointLogic)(const struct Endpoint* e,char *arg1, char *arg2);
};

typedef struct Endpoint endpoint_t;

// endpoint library functions
endpoint_t *createEndpoint(char *name, char *commandLineArg);
void setDocumentationString(struct Endpoint* e, char *docBuffer);

//help menu
void DTunesHelpMenu(struct Endpoint *e, char *myarg, char *myarg2);

// playlist functions
void insertPlaylistCmd(struct Endpoint *e,char *myarg, char *myarg2);
void viewPlaylistCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void deletePlaylistCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void deleteAllPlaylistsCmd(struct Endpoint *e, char *myarg, char *myarg2); 

// test playlist functions
void testInsertPlaylist(struct Endpoint *e, char *myarg, char *myarg2);
void testLoadPlaylists(struct Endpoint *e, char *myarg, char *myarg2);
void testViewPlaylists(struct Endpoint *e, char *myarg, char *myarg2); 
void testDeletePlaylist(struct Endpoint *e, char *myarg, char *myarg2);
void testDeletePlaylists(struct Endpoint *e, char *myarg, char *myarg2);

// song functions
void insertSongCmd(struct Endpoint *e, char *myarg, char *myarg2); 
void viewSongsCmd(struct Endpoint *e, char *myarg);
void deleteSongCmd(struct Endpoint *e, char *myarg, char *myarg2);
void deleteAllSongsCmd(struct Endpoint *e, char *myarg); 
 
// test song functions
void testInsertSong(struct Endpoint *e, char *myarg, char *myarg2);
void testUpdateSong(char *myarg, char *myarg2, char *myarg3);
void testLoadSongs(struct Endpoint *e, char *myarg, char *myarg2);
void testViewSongs(struct Endpoint *e, char *myarg, char *myarg2);
void testDeleteSong(struct Endpoint *e, char *myarg, char *myarg2);
void testDeleteSongs(struct Endpoint *e, char *myarg, char *myarg2);




 #endif 
