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

#define DB_PATH "data/db/dtunes.db"
#define BACKUP_DB_PATH  "data/db/dtunes_backup.db"
#define AUDIO_FILE_PATH  "data/audiofiles"
#define YOUTUBE_FILE_PATH  "data/youtubedl"

// queries for playlist
#define INSERT_DB_PLAYLISTS  "INSERT INTO PLAYLIST VALUES(?,?,?)"
#define ADD_SONG_PLAYLIST "INSERT INTO PLAYLIST_SONG VALUES(?,?,?)"
#define VIEW_DB_PLAYLISTS  "SELECT * FROM PLAYLIST" 
#define VIEW_PLAYLIST_UUID "SELECT * FROM PLAYLIST WHERE playlist_uuid=?"
#define VIEW_PLAYLIST_BY_NAME "SELECT * FROM PLAYLIST WHERE name=?"
#define VIEW_PLAYLIST_SONGS "SELECT * FROM PLAYLIST_SONG WHERE playlist=?"
#define DELETE_DB_PLAYLIST  "DELETE FROM PLAYLIST where name=?" 
#define DELETE_DB_PLAYLIST_UUID  "DELETE FROM PLAYLIST where playlist_uuid=?" 
#define DELETE_DB_PLAYLISTS  "DELETE  FROM PLAYLIST"
#define DELETE_PLAYLIST_RELATIONS "DELETE FROM PLAYLIST_SONG"
#define DELETE_PLAYLIST_RELATION_UUID "DELETE FROM PLAYLIST_SONG WHERE playlist=?"
#define COUNT_PLAYLIST_RELATIONS "SELECT COUNT(*) FROM PLAYLIST_SONG WHERE playlist=?"
#define COUNT_DB_PLAYLISTS "SELECT COUNT(*) FROM PLAYLIST"

// queries for songs
#define INSERT_DB_SONGS  "INSERT INTO SONG VALUES(?,?,?,?,?,?)"
#define VIEW_DB_SONGS  "SELECT * FROM SONG"
#define VIEW_SONG_UUID "SELECT * FROM SONG WHERE song_uuid=?"
#define VIEW_SONG_NAME "SELECT * FROM SONG WHERE name=?"
#define UPDATE_SONG_BY_NAME "UPDATE SONG SET name=? where name=?" // not using yet
#define UPDATE_SONG_BY_UUID "UPDATE SONG SET name=? where song_uuid=?" // not using yet
#define DELETE_DB_SONG_UUID  "DELETE FROM SONG where song_uuid=?" 
#define DELETE_DB_SONG_NAME  "DELETE FROM SONG where name=?" 
#define DELETE_DB_SONGS  "DELETE  FROM SONG"
#define COUNT_DB_SONGS "SELECT COUNT(*) FROM SONG"

// youtube url
#define INSERT_DB_URL "INSERT INTO YOUTUBE_URL VALUES(?,?,?)"
#define VIEW_DB_URLS "SELECT * FROM YOUTUBE_URL"
#define COUNT_DB_URLS "SELECT COUNT(*) FROM YOUTUBE_URL"
#define DELETE_DB_URL  "DELETE FROM YOUTUBE_URL where yt_url=?"
#define DELETE_DB_URL_UUID  "DELETE FROM YOUTUBE_URL where url_uuid=?" 
#define DELETE_DB_URLS  "DELETE  FROM YOUTUBE_URL"


// collections queries
#define INSERT_DB_COLLECTION  "INSERT INTO COLLECTION VALUES(?,?,?,?,?)"



sqlite3* openDB(char *filename);
char* getCurrentTime();

// logging functions
void d_log(char* type, char *message); 
void dlog(char *type, char*message); 
void dlog_int(char *type, int message); 
void dlog_counter(char *type, char *message, int counter); 
void d_log_time(char* type, char* message); 
void generateBanner(int amount);

// youtube dl functions
int downloadVideo(char *url);
void backupVideo(char *dbUrl); 

// file db functions
char* combineFileStrs(const char *cwd, const char *fileName); 
void removeChar(char *testString, char charToRemove); 
void getCurrentDirectory(char *cwd); 
void renameFile(char *fileName, char *newFileName); 
int countFiles(char *directoryPath); 
//void reformatFileNames(char *desiredPath);
void clearAudioFileDirectory(char *desiredPath); 

#endif 
