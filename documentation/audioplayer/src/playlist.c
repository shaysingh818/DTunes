#include "playlist.h"
//#include "song.h"
#include "db.h"



playlist_t **initPlaylists(int limit){
/**
	Function for returning structure instance of a song
*/
	playlist_t **playlists = malloc(limit * sizeof(playlist_t*)); 			
	// allocate space
	for(int i = 0; i < limit; i++){
		playlists[i] = malloc(sizeof(playlist_t)); 
	}

	// open db
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement	
	sqlite3_stmt *sql; 
	char *query = VIEW_DB_PLAYLISTS; 
	// prepare statement
	int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
	// check for errors
  	if(result != SQLITE_OK){
        fprintf(stderr, "Failed to view playlists:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
    }


	int indexCount = 0; 
	// allocate results into struct array
	while ((result = sqlite3_step(sql)) == SQLITE_ROW) {
		// extract column values	
		const char *column0 = sqlite3_column_text(sql, 0); 	
		const char *column1 = sqlite3_column_text(sql, 1); 	
		const char *column2 = sqlite3_column_text(sql, 2); 
	
		// parse uuid from string
		uuid_parse(column0, playlists[indexCount]->playlistId); 	
		// store values:	
		strcpy(playlists[indexCount]->name, column1); 	
		strcpy(playlists[indexCount]->dateCreated, column2); 
		indexCount += 1;
	}
	

	sqlite3_close(db);
	return playlists;  
	
}



int retrieveLastPlaylistId(){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	char *errMsg = 0; 
	sqlite3_stmt *sql; 
	
	int lastId = sqlite3_last_insert_rowid(db); 	
	printf("Last insert row id: %d\n", lastId);
	// return last inserted row
	return lastId; 
}


int getPlaylistTableSize(){
	// get amount of playlists in db    
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = COUNT_DB_PLAYLISTS;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query playlists:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

	// load playlist limit
    int playlistLimit;
    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){
        const char *columnValue = sqlite3_column_text(sql, 0);
        sscanf(columnValue, "%d", &playlistLimit); // Using sscanf
    }

	// close db
	sqlite3_finalize(sql);
    sqlite3_close(db);
	return playlistLimit; 
}

int createPlaylist(playlist_t* playlist){
	// open db	
	sqlite3 *db = openDB(DB_PATH);
	// prepare statement	
	char *errMsg = 0; 
	sqlite3_stmt *sql;	 
	int result = sqlite3_prepare_v2(db, "INSERT INTO PLAYLIST VALUES(?, ?, ?)", -1, &sql, NULL);		 

	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to insert:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return 0; 
	}

	char playlist_uuid[37]; 
	uuid_generate_time_safe(playlist->playlistId); 
	uuid_unparse_lower(playlist->playlistId, playlist_uuid); 	
	sqlite3_bind_text(sql, 1, playlist_uuid, -1, NULL);	
	sqlite3_bind_text(sql, 2, playlist->name, -1, NULL);	
	sqlite3_bind_text(sql, 3, playlist->dateCreated, -1, NULL);

	// do first insert
	sqlite3_step(sql);
	sqlite3_close(db);

	return 1;  
}

int viewPlaylists(){
	// get amount of playlists in db    
	// Call the load songs with limit
	int playlistLimit = getPlaylistTableSize(); 
    playlist_t **playlists = initPlaylists(playlistLimit);
    playlist_t ***p = &playlists;

    // print header 
    printf("\n");
	printf("\e[0;31m");
	printf("%-45s %-25s %-15s\n", "UUID", "Name", "Date");
	generateBanner(100);  
    // View song in format for terminal
    for(int i = 0; i < playlistLimit; i++){
		char playlist_uuid[37]; 
		uuid_unparse_lower((*p)[i]->playlistId, playlist_uuid);
        printf("%-45s %-25s %-15s", playlist_uuid, (*p)[i]->name, (*p)[i]->dateCreated);
    }
    printf("\n");

    // return true or false if view was successful
    return TRUE;



}

int deletePlaylist(char *playlistName){	
	// open db
	sqlite3 *db = openDB(DB_PATH);

	// prepare statement
	sqlite3_stmt *sql; 
	
	// prepare statement
	int result = sqlite3_prepare_v2(db, DELETE_DB_PLAYLIST, -1, &sql, NULL);
	// bind vars to statement
	sqlite3_bind_text(sql, 1, playlistName, -1, NULL);
	 
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return FALSE; 
	}

	printf("[DB OPERATION] Deleted all playlists\n"); 
	sqlite3_step(sql); 
	sqlite3_close(db); 

	return TRUE; 
}


int deleteAllPlaylists(){	
	 // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql; 

    char *query = DELETE_DB_PLAYLISTS; 
    char *errMsg = 0; 
    
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL); 
    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete all playlists:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE; 
    }

	printf("[DB OPERATION] ]DELETE ALL PLAYLISTS\n");
    sqlite3_step(sql); 
    sqlite3_close(db); 

    return TRUE; 

}


int checkPlaylistExist(char *playlistName){	
	// open db
	sqlite3 *db = openDB(DB_PATH);
	
	// check if playlist exists

	sqlite3_stmt *res;	
	char *sql = DELETE_DB_PLAYLIST; 
	char *err_msg = 0;
 
	// print headers
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0); 
	if(rc == SQLITE_OK){
		sqlite3_bind_text(res, 1, playlistName, -1, NULL);
	}else {   
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }
	
	// check if playlist exists
	int row = sqlite3_step(res);
	if(row == SQLITE_ROW){
		return TRUE; 
	}

	sqlite3_finalize(res);
	sqlite3_close(db); 

	return FALSE; 
}


