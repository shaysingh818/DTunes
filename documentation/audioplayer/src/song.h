#ifndef SONG_H
#define SONG_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <uuid/uuid.h>

/**
	Model for storing audio files
*/
struct AudioFile {
	uuid_t songId; 
	char name[100];
	char dateCreated[30]; 
	char filePath[100];
	char subtitles[100]; 
	int plays;  
};

typedef struct AudioFile song_t; 



// callbacks
static int callback(void *data, int argc, char **argv, char **azColName); 

// db functions
song_t **initSongs(int limit);
int getSongTableSize(); 
void insertSong(char *fileName, char *currentTime, char *streamingPath);
int createSong(song_t* newSong);
int viewSongs();  
int updateSong(char *prevSongName, char *newSongName); 
int deleteSong(char *songName); 
int deleteAllSongs();  
int checkSongExist(char *songName); 

/**
 * Functions for syncing audio files to a sqlite3 database
 * Also functions that extract data from file directories
 * */
int syncDirectoryInformation(char *filePath); 
int loadAudioFilesFromDirectory(char *filePath);


 

#endif 
