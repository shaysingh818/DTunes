#include "song.h"
#include "../db/db.h"


static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


void printSong(song_t *mySong){
	dlog("SONG NAME", mySong->name); 
	dlog("DATE CREATED", mySong->dateCreated); 
	dlog("FILE PATH", mySong->filePath); 
	dlog("SUBTITLES", mySong->subtitles);
	dlog_int("SONG PLAYS", mySong->plays);  
}


song_t **initSongs(int limit){
/**
	Function for returning structure instance of a song
*/
	song_t **songs = malloc(limit * sizeof(song_t*)); 			
	// allocate space
	for(int i = 0; i < limit; i++){
		songs[i] = malloc(sizeof(song_t)); 
	}

	// open db
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement	
	sqlite3_stmt *sql; 
	char *query = VIEW_DB_SONGS; 
	// prepare statement
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
	// check for errors
  	if(result != SQLITE_OK){
        fprintf(stderr, "Failed to view songs:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
    }


	int indexCount = 0; 
	// allocate results into struct array
	while ((result = sqlite3_step(sql)) == SQLITE_ROW) {
		// extract column values	
		const char *column0 = sqlite3_column_text(sql, 0); 	
		const char *column1 = sqlite3_column_text(sql, 1); 	
		const char *column3 = sqlite3_column_text(sql, 2); 
		const char *column4 = sqlite3_column_text(sql, 3); 
		const char *column6 = sqlite3_column_text(sql, 4); 
		const char *column7 = sqlite3_column_text(sql, 5);
		
		int songPlays; 
		// convert column 7 to int
		sscanf(column7, "%d", &songPlays); // Using sscanf
		uuid_parse(column0, songs[indexCount]->songId);
		// store values
		strcpy(songs[indexCount]->name, column1); 
		strcpy(songs[indexCount]->dateCreated, column3); 
		strcpy(songs[indexCount]->filePath, column4); 
		strcpy(songs[indexCount]->subtitles, column6);
		songs[indexCount]->plays = songPlays; 	
		indexCount += 1;
	}
	
	sqlite3_close(db);
	return songs;  
	
}



song_t *viewSongById(char *uuid){
	// view song by id
	song_t *song;	
    song = (song_t*)malloc(sizeof(song_t));

	sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_SONG_UUID;
	
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query song by uuid:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
	
	// bind uuid to query
	sqlite3_bind_text(sql, 1, uuid, -1, NULL); 

	result = sqlite3_step(sql); 
	if(result == SQLITE_ROW){

		const char *songUuid = sqlite3_column_text(sql, 0);
		const char *songName = sqlite3_column_text(sql, 1);
		const char *songDate = sqlite3_column_text(sql, 2);	
		const char *songFilePath = sqlite3_column_text(sql, 3);	
		const char *songSubtitles = sqlite3_column_text(sql, 4);	
		const char *songPlays = sqlite3_column_text(sql, 5);
		int songP; 

		// debug fields
		sscanf(songPlays, "%d", &songP); // Using sscanf
        uuid_parse(songUuid, song->songId);

        // store values
        strcpy(song->name, songName);
        strcpy(song->dateCreated, songDate);
        strcpy(song->filePath, songFilePath);
        strcpy(song->subtitles, songSubtitles);
		song->plays = songP; 
	}

	sqlite3_finalize(sql); 
	sqlite3_close(db); 
	return song; 
}




song_t *viewSongByName(char *songName){
	// view song by id
	song_t *song;	
    song = (song_t*)malloc(sizeof(song_t));

	sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_SONG_NAME;
	
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query song by name:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
	
	// bind uuid to query
	sqlite3_bind_text(sql, 1, songName, -1, NULL); 

	result = sqlite3_step(sql); 
	if(result == SQLITE_ROW){

		const char *songUuid = sqlite3_column_text(sql, 0);
		const char *songName = sqlite3_column_text(sql, 1);
		const char *songDate = sqlite3_column_text(sql, 2);	
		const char *songFilePath = sqlite3_column_text(sql, 3);	
		const char *songSubtitles = sqlite3_column_text(sql, 4);	
		const char *songPlays = sqlite3_column_text(sql, 5);
		int songP; 

		// debug fields
		sscanf(songPlays, "%d", &songP); // Using sscanf
        uuid_parse(songUuid, song->songId);

        // store values
        strcpy(song->name, songName);
        strcpy(song->dateCreated, songDate);
        strcpy(song->filePath, songFilePath);
        strcpy(song->subtitles, songSubtitles);
		song->plays = songP; 
	}

	sqlite3_finalize(sql); 
	sqlite3_close(db); 
	return song; 
}


int getSongTableSize(){
	// get amount of songs in db	
	sqlite3 *db = openDB(DB_PATH);
	sqlite3_stmt *sql; 
	char *query = COUNT_DB_SONGS; 	
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);	
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return FALSE; 
	}

	// load song limit
	int songLimit;
	result = sqlite3_step(sql);
	if(result == SQLITE_ROW){
		const char *columnValue = sqlite3_column_text(sql, 0); 
		sscanf(columnValue, "%d", &songLimit); // Using sscanf
	}

	sqlite3_finalize(sql); 
	sqlite3_close(db); 
	
	return songLimit; 
}



void insertSong(char *fileName, char *currentTime, char *streamingPath){
    song_t *newSong;
    newSong = (song_t*)malloc(sizeof(song_t));
    printf("\033[0;32m");

    // set values
    strcpy(newSong->name, fileName);
    strcpy(newSong->dateCreated, currentTime);
    strcpy(newSong->filePath, streamingPath);
    strcpy(newSong->subtitles, "");
    newSong->plays = 0;
	
	// generate uuid
	char song_uuid[37];
    uuid_generate_time_safe(newSong->songId);
    uuid_unparse_lower(newSong->songId, song_uuid);

    int dbResult = createSong(newSong);
    if(dbResult){
		if(DEBUG == TRUE){
        	d_log_time("DB INSERT", "INSERT SONG");
		}
    }else{
        d_log_time("FAILED", "INSERT SONG FAILED");
    }
}



int createSong(song_t* newSong){	
	// open db

	char cwd[256];
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        dlog("ERROR", "CURRENT WORKING DIRECTORY");
    }

    strcat(cwd, "/");

	sqlite3 *db = openDB(DB_PATH);
	// prepare statement	
	char *errMsg = 0; 
	sqlite3_stmt *sql;	 
	char *query = INSERT_DB_SONGS;
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
			 
	// get current time
	char* mytime = getCurrentTime(); 

	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to insert:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return 0; 
	}

	char song_uuid[37];
    uuid_unparse_lower(newSong->songId, song_uuid);
	
	// bind song variables to sqlite3 statment	
	sqlite3_bind_text(sql, 1, song_uuid, -1, NULL);	
	sqlite3_bind_text(sql, 2, newSong->name, -1, NULL);	
	sqlite3_bind_text(sql, 3, newSong->dateCreated, -1, NULL);	
	sqlite3_bind_text(sql, 4, newSong->filePath, -1, NULL);
	sqlite3_bind_text(sql, 5, newSong->subtitles, -1, NULL);	
	sqlite3_bind_int(sql, 6, newSong->plays);	

	// do first insert
	sqlite3_step(sql);
	sqlite3_close(db);
	
	return 1;  
}

int viewSongs(){

    // Call the load songs with limit   
    int songLimit = getSongTableSize();
    song_t **songs = initSongs(songLimit);
    song_t ***p = &songs;

    // print header 
    printf("\n");
    printf("\e[0;31m");
    printf("%-45s %-25s\n", "UUID", "Name");
    generateBanner(80);
    // View song in format for terminal
    for(int i = 0; i < songLimit; i++){
        char song_uuid[37];
        uuid_unparse_lower((*p)[i]->songId, song_uuid);
        printf("%-45s %-24s\n", song_uuid ,(*p)[i]->name);
    }
    printf("\n");

    // return true or false if view was successful
    return TRUE;
}



int updateSongByName(char *prevSongName, char *newSongName){	
	// open db
	char *sql;
	sqlite3 *db = openDB(DB_PATH);	
	char *errMsg = 0;
	const char* data = "Callback function called";
	// prepare statement
	char string[200]; 	
	sprintf(string, "UPDATE SONG set name ='%s' WHERE name='%s'; ", newSongName, prevSongName);
	sql = string; \
			"SELECT * from SONG";   

	int result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);
	
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",errMsg);
		 sqlite3_free(errMsg);
		return FALSE; 
	}
	
	//sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
}



int updateSongById(char *uuid, char *newSongName){	
	// open db
	char *sql;
	sqlite3 *db = openDB(DB_PATH);	
	char *errMsg = 0;
	const char* data = "Callback function called";
	// prepare statement
	char string[200]; 	
	sprintf(string, "UPDATE SONG set name ='%s' WHERE song_uuid='%s'; ", newSongName, uuid);
	sql = string; \
			"SELECT * from SONG";   

	int result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);
	
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",errMsg);
		 sqlite3_free(errMsg);
		return FALSE; 
	}
	
	//sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
}



int deleteSongByName(char *songName){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement
	sqlite3_stmt *sql; 
	char *query = DELETE_DB_SONG_NAME; 
	// prepare statement
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
	// bind vars to statement
	sqlite3_bind_text(sql, 1, songName, -1, NULL);
	 
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return FALSE; 
	}

	sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
}



int deleteSongById(char *songUuid){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement
	sqlite3_stmt *sql; 
	char *query = DELETE_DB_SONG_UUID; 
	// prepare statement
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
	// bind vars to statement
	sqlite3_bind_text(sql, 1, songUuid, -1, NULL);
	 
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return FALSE; 
	}

	sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
}


int deleteAllSongs(){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	sqlite3_stmt *sql; 

	char *query = DELETE_DB_SONGS;
	char *errMsg = 0; 
	
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);	
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return FALSE; 
	}

	sqlite3_step(sql); 
	sqlite3_close(db);

	clearAudioFileDirectory(AUDIO_FILE_PATH);  

	return TRUE; 
}



int checkSongExist(char *songName){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	// check if playlist exists

	sqlite3_stmt *res;	
	char *sql = DELETE_DB_SONG_NAME; 
	char *err_msg = 0;
 
	// print headers
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0); 
	if(rc == SQLITE_OK){
		sqlite3_bind_text(res, 1, songName, -1, NULL);
	}else {   
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
	
	// check if playlist exists
	int row = sqlite3_step(res);
	if(row == SQLITE_ROW){
		return TRUE; 
	}

	return FALSE; 
}


// function to sync directory info to song struct array
int syncDirectoryInformation(char *filePath){

 	DIR *folder;
    struct dirent *entry;
    int files = 0; 
    folder = opendir(filePath);
    if(folder == NULL){
    	perror("Unable to read directory\n");
    } 
	
	char cwd[256]; 
	if(getcwd(cwd, sizeof(cwd)) == NULL){
		dlog("ERROR", "CURRENT WORKING DIRECTORY"); 
	}
	strcat(cwd, "/");

    while((entry=readdir(folder))){	
		files++;
		// file information
		char *tempFileName = entry->d_name;
		char *currTime = getCurrentTime();

		// file name processing
		removeChar(tempFileName, ' ');  	
		removeChar(tempFileName, ',');  
		removeChar(tempFileName, '\'');  
	
		// get previous file name 
		char *idk = malloc(strlen(tempFileName) + 1); 
		strcpy(idk, tempFileName); 

		// extract desired audiofile streaming location
		char *streamingPath = combineFileStrs(cwd, tempFileName);

		int fileCondition1 = strcmp(entry->d_name, "..") == 0; 
		int fileCondition2 = strcmp(entry->d_name, ".") == 0;

		if(fileCondition1 == 0 & fileCondition2 == 0){
			insertSong(entry->d_name, currTime, streamingPath);
			dlog("SYNC SONG", tempFileName); 
		}
	}

	closedir(folder);

	return TRUE; 
	
}



/**
 * Reads directory information then loads it into a struct array of songs
 * */
int loadAudioFilesFromDirectory(char *filePath){	
	// Syncs all info to db
	int result = syncDirectoryInformation(filePath);
	if(result){
		return TRUE; 
	}
	return FALSE; 
}


