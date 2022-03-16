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

//help menu
void DTunesHelpMenu(char *myarg);

// playlist functions
void insertPlaylistCmd(char *myarg, char *myarg2);
void viewPlaylistCmd(char *myarg); 
void deletePlaylistCmd(char *myarg, char *myarg2); 
void deleteAllPlaylistsCmd(char *myarg); 

// song functions
void insertSongCmd(char *myarg, char *myarg2); 
void viewSongsCmd(char *myarg);
void deleteSongCmd(char *myarg, char *myarg2);
void deleteAllSongsCmd(char *myarg); 
 

 #endif 
