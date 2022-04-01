#include "song.h"
#include "db.h"


static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
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
		const char *column1 = sqlite3_column_text(sql, 0); 	
		const char *column3 = sqlite3_column_text(sql, 1); 
		const char *column4 = sqlite3_column_text(sql, 2); 
		const char *column6 = sqlite3_column_text(sql, 3); 
		const char *column7 = sqlite3_column_text(sql, 4);
		
		int songPlays; 
		// convert column 7 to int
		sscanf(column7, "%d", &songPlays); // Using sscanf
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
    d_log_time("STRUCTURE ALLOCATION", "CREATED SONG");

    // set values
    strcpy(newSong->name, fileName);
    strcpy(newSong->dateCreated, currentTime);
    strcpy(newSong->filePath, streamingPath);
    strcpy(newSong->subtitles, "");
    newSong->plays = 0;

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
	// bind song variables to sqlite3 statment
	sqlite3_bind_text(sql, 1, newSong->name, -1, NULL);	
	sqlite3_bind_text(sql, 2, newSong->dateCreated, -1, NULL);	
	sqlite3_bind_text(sql, 3, newSong->filePath, -1, NULL);
	sqlite3_bind_text(sql, 4, newSong->subtitles, -1, NULL);	
	sqlite3_bind_int(sql, 5, newSong->plays);	

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
	printf("Name             						Date\n");
	printf("================================================================================\n");
	// View song in format for terminal
	for(int i = 0; i < songLimit; i++){
		printf("%-55s %s\n", (*p)[i]->name, (*p)[i]->dateCreated); 
	}
	printf("\n"); 

	// return true or false if view was successful
	return TRUE; 
}


int updateSong(char *prevSongName, char *newSongName){	
	// open db
	char *sql;
	sqlite3 *db = openDB(DB_PATH);	
	char *errMsg = 0;
	const char* data = "Callback function called";
	// prepare statement
	char string[200]; 	
	sprintf(string, "UPDATE SONG set name ='%s' WHERE name='%s'; ", prevSongName, newSongName);
	sql = string; \
			"SELECT * from SONG";   

	dlog("QUERY TEST", sql);  
	int result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);
	
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",errMsg);
		 sqlite3_free(errMsg);
		return FALSE; 
	}
	
	printf("[DB OPERATION] UPDATED SONG\n");
	//sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
}



int deleteSong(char *songName){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement
	sqlite3_stmt *sql; 
	char *query = DELETE_DB_SONG; 
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


	printf("[DB OPERATION] Deleted song\n");
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

	printf("[DB OPERATION]: DELETE ALL SONGS\n"); 
	sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
}



int checkSongExist(char *songName){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	// check if playlist exists

	sqlite3_stmt *res;	
	char *sql = DELETE_DB_SONG; 
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
	
	// change to desired directory
	if(chdir(filePath) != 0){	
		dlog("ERROR", "CHANGE DIR"); 
	}

	// get current working directory	
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

		// get second current working directory
		dlog("CURRENT DIR", cwd);

		// change dir for db insert	
		if(chdir("/home/dread/Desktop/DTunes/src") != 0){	
			dlog("ERROR", "CHANGE DIR"); 
			return FALSE; 
		}

		int fileCondition1 = strcmp(entry->d_name, "..") == 0; 
		int fileCondition2 = strcmp(entry->d_name, ".") == 0;
		dlog("FILENAME", entry->d_name);  
		dlog_int("FILE CONDITION", fileCondition1); 
		dlog_int("FILE CONDITION 2 ", fileCondition2); 
		if(fileCondition1 == 0 & fileCondition2 == 0){
			insertSong(entry->d_name, currTime, streamingPath);
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



url_t **initUrls(int limit){
/**
	Function for returning structure instance of a song
*/
	url_t **urls = malloc(limit * sizeof(url_t*)); 			
	// allocate space
	for(int i = 0; i < limit; i++){
		urls[i] = malloc(sizeof(url_t)); 
	}

	// open db
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement	
	sqlite3_stmt *sql; 
	char *query = VIEW_DB_URLS; 
	// prepare statement
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
	// check for errors
  	if(result != SQLITE_OK){
        fprintf(stderr, "Failed to view youtube urls:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
    }


	int indexCount = 0; 
	// allocate results into struct array
	while ((result = sqlite3_step(sql)) == SQLITE_ROW) {
		// extract column values
		const char *column1 = sqlite3_column_text(sql, 0); 	
		const char *column2 = sqlite3_column_text(sql, 1); 	
		// store values
		strcpy(urls[indexCount]->url, column1); 
		strcpy(urls[indexCount]->dateCreated, column2); 
		indexCount += 1;
	}
	

	sqlite3_close(db);
	return urls;  
	
}



int getUrlTableSize(){
	// get amount of songs in db	
	sqlite3 *db = openDB(DB_PATH);
	sqlite3_stmt *sql; 
	char *query = COUNT_DB_URLS; 	
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);	
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete song:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return FALSE; 
	}

	// load song limit
	int urlLimit;
	result = sqlite3_step(sql);
	if(result == SQLITE_ROW){
		const char *columnValue = sqlite3_column_text(sql, 0); 
		sscanf(columnValue, "%d", &urlLimit); // Using sscanf
	}

	sqlite3_finalize(sql); 
	sqlite3_close(db); 
	
	return urlLimit; 
}


int insertUrl(char *url, char *currentTime){
    url_t *newUrl;
    newUrl = (url_t*)malloc(sizeof(url_t));
    printf("\033[0;32m");
    d_log_time("STRUCTURE ALLOCATION", "CREATED YOUTUBE URL");

    // set values
    strcpy(newUrl->url, url);
    strcpy(newUrl->dateCreated, currentTime);

    int dbResult = createYoutubeUrl(newUrl);
    if(dbResult){
		if(DEBUG == TRUE){
        	d_log_time("DB INSERT", "INSERT YOUTUBE URL");
			return TRUE; 
		}
    }else{
        d_log_time("FAILED", "INSERT YOUTUBE URL FAILED");
		return FALSE; 
    }
}


int createYoutubeUrl(url_t* newUrl){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement	
	char *errMsg = 0; 
	sqlite3_stmt *sql;	 
	char *query = INSERT_DB_URL;
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
			 
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to insert:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return 0; 
	}
	// bind song variables to sqlite3 statment
	sqlite3_bind_text(sql, 1, newUrl->url, -1, NULL);	
	sqlite3_bind_text(sql, 2, newUrl->dateCreated, -1, NULL);	

	// do first insert
	sqlite3_step(sql);
	sqlite3_close(db);
	
	return 1;  
}


int viewUrls(){

	// Call the load songs with limit	
	int urlLimit = getUrlTableSize(); 
	url_t **urls = initUrls(urlLimit); 
	url_t ***p = &urls;  

	// print header	
	printf("\n"); 
	printf("Url             						Date\n");
	printf("================================================================================\n");
	// View song in format for terminal
	for(int i = 0; i < urlLimit; i++){
		printf("%-55s %s\n", (*p)[i]->url, (*p)[i]->dateCreated); 
	}
	printf("\n"); 

	// return true or false if view was successful
	return TRUE; 
}
