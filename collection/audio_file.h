#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <time.h>
#include <uuid/uuid.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>


/* audio file db queries */
#define INSERT_DB_FILE "INSERT INTO AUDIO_FILE VALUES(?,?,?)"
#define VIEW_DB_FILES "SELECT * FROM AUDIO_FILE"
#define VIEW_DB_FILE "SELECT * FROM AUDIO_FILE WHERE name=?"
#define COUNT_DB_FILE "SELECT COUNT(*) FROM AUDIO_FILE"
#define DELETE_DB_FILE "DELETE FROM AUDIO_FILE WHERE name=?" 
#define DELETE_DB_FILES "DELETE  FROM AUDIO_FILE"


struct AudioFile {
    char *name;
    char *dateCreated;
    char *streamingPath;
};

typedef struct AudioFile audiofile_t;


audiofile_t **initAudioFiles(int limit);
audiofile_t *viewAudioFile(char *name);
int createAudioFile(char *setName, char *setStreamingPath);
int updateAudioFileById(char *uuid, char *newName);
int getAudioFileTableSize();
int viewAudioFiles();
int deleteAudioFile(char *name);
int deleteAllAudioFiles();
int checkAudioFileExists(char *name);


#endif
