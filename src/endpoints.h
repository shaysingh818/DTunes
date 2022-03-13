#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>

#define HELP_MENU  "help"

// playlists
#define INSERT_PLAYLIST  "create-p"
#define VIEW_PLAYLIST  "view-p"
#define DELETE_PLAYLIST  "delete-p" 
#define DELETE_ALL_PLAYLIST  "delete-ps"

//songs
#define INSERT_SONG  "create-s"
#define VIEW_SONG  "view-s"
#define DELETE_SONG  "delete-s" 
#define DELETE_ALL_SONG  "delete-ss"

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
