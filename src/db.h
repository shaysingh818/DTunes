#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE

#define DB_PATH "../data/db/dtunes.db"
#define BACKUP_DB_PATH  "../data/db/dtunes_backup.db"
#define AUDIO_FILE_PATH  "../data/audiofiles"
#define YOUTUBE_FILE_PATH  "../data/youtubedl"

// queries for playlist
#define VIEW_DB_PLAYLISTS  "SELECT * FROM PLAYLIST" 
#define INSERT_DB_PLAYLISTS  "INSERT INTO PLAYLIST VALUES(?,?)"
#define DELETE_DB_PLAYLIST  "DELETE FROM PLAYLIST where name=?" 
#define DELETE_DB_PLAYLISTS  "DELETE  FROM PLAYLIST"
#define COUNT_DB_PLAYLISTS "SELECT COUNT(*) FROM PLAYLIST"

// queries for songs
#define INSERT_DB_SONGS  "INSERT INTO SONG VALUES(?,?,?,?,?)"
#define VIEW_DB_SONGS  "SELECT * FROM SONG"
#define UPDATE_SONG "UPDATE SONG SET name=? where name=?"
#define DELETE_DB_SONG  "DELETE FROM SONG where name=?" 
#define DELETE_DB_SONGS  "DELETE  FROM SONG"
#define COUNT_DB_SONGS "SELECT COUNT(*) FROM SONG"

// youtube url
#define INSERT_DB_URL "INSERT INTO YOUTUBE_URL VALUES(?, ?, ?)"
#define VIEW_DB_URLS "SELECT * FROM YOUTUBE_URL"
#define COUNT_DB_URLS "SELECT COUNT(*) FROM YOUTUBE_URL"

sqlite3* openDB(char *filename);
char* getCurrentTime();

// logging functions
void d_log(char* type, char *message); 
void dlog(char *type, char*message); 
void dlog_int(char *type, int message); 
void dlog_counter(char *type, char *message, int counter); 
void d_log_time(char* type, char* message); 


// youtube dl functions
int insertUrl(char *url, char *desiredPath); 
int callPyScript(char *url);
int downloadVideo(char *url);


// file db functions
char* combineFileStrs(const char *cwd, const char *fileName); 
void removeChar(char *testString, char charToRemove); 
void getCurrentDirectory(char *cwd); 
void renameFile(char *fileName, char *newFileName); 
int countFiles(char *directoryPath); 
void reformatFileNames(char *desiredPath);


#endif 
