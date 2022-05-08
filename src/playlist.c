#include "playlist.h"
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
		
		uuid_parse(column0, playlists[indexCount]->playlistId); 
		// store values:
		strcpy(playlists[indexCount]->name, column1); 	
		strcpy(playlists[indexCount]->dateCreated, column2); 
		indexCount += 1;
	}
	

	sqlite3_close(db);
	return playlists;  
	
}


playlist_t *viewPlaylistById(char *uuid){
    // view song by id
    playlist_t *playlist;
    playlist = (playlist_t*)malloc(sizeof(playlist_t));

    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_PLAYLIST_UUID; 

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

        const char *playlistUuid = sqlite3_column_text(sql, 0);
        const char *playlistName = sqlite3_column_text(sql, 1);
        const char *playlistDate = sqlite3_column_text(sql, 2);

        // debug fields
        uuid_parse(playlistUuid, playlist->playlistId);
        // store values
        strcpy(playlist->name, playlistName);
        strcpy(playlist->dateCreated, playlistDate);

    }

    sqlite3_finalize(sql);
    sqlite3_close(db);
    return playlist;
}



playlist_t *viewPlaylistByName(char *playlistName){
    // view song by id
    playlist_t *playlist;
    playlist = (playlist_t*)malloc(sizeof(playlist_t));

    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = VIEW_PLAYLIST_BY_NAME; 

    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query song by uuid:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
    
    // bind uuid to query
    sqlite3_bind_text(sql, 1, playlistName, -1, NULL);

    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){

        const char *playlistUuid = sqlite3_column_text(sql, 0);
        const char *playlistName = sqlite3_column_text(sql, 1);
        const char *playlistDate = sqlite3_column_text(sql, 2);

        // debug fields
        uuid_parse(playlistUuid, playlist->playlistId);
        // store values
        strcpy(playlist->name, playlistName);
        strcpy(playlist->dateCreated, playlistDate);

    }

    sqlite3_finalize(sql);
    sqlite3_close(db);
    return playlist;
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


int deleteCascadingPlaylists(char *playlistUuid){
	
	 // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql; 

    char *query = DELETE_PLAYLIST_RELATION_UUID; 
    char *errMsg = 0; 
    
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL); 
    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete all playlist relations:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE; 
    }
	
    sqlite3_bind_text(sql, 1, playlistUuid, -1, NULL);	

    sqlite3_step(sql); 
    sqlite3_close(db); 

    return TRUE; 
}


int deletePlaylist(char *playlistUuid){	
	// open db
	sqlite3 *db = openDB(DB_PATH);

	// prepare statement
	sqlite3_stmt *sql; 
	
	// prepare statement
	int result = sqlite3_prepare_v2(db, DELETE_DB_PLAYLIST_UUID, -1, &sql, NULL);
	// bind vars to statement
	sqlite3_bind_text(sql, 1, playlistUuid, -1, NULL);
	 
	// check for sql cursor errors
	if(result != SQLITE_OK){
		fprintf(stderr, "Failed to delete:  %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return FALSE; 
	}

	sqlite3_step(sql); 
	sqlite3_close(db);

	int dbresult = deleteCascadingPlaylists(playlistUuid); 
	if(!dbresult){
		dlog("FAILED", "CASCADING DB DELETE"); 
	}

	return TRUE; 
}


int deletePlaylistByName(char *playlistName){
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

    sqlite3_step(sql);
    sqlite3_close(db);

    return TRUE;
}


int deleteAllPlaylistRelations(){	
	 // open db
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql; 

    char *query = DELETE_PLAYLIST_RELATIONS; 
    char *errMsg = 0; 
    
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL); 
    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to delete all playlist relations:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE; 
    }

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

    sqlite3_step(sql); 
    sqlite3_close(db); 
	
	int dbresult = deleteAllPlaylistRelations(); 
	if(!dbresult){
		return FALSE; 
	}

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


int getRelationTableSize(char *playlistUuid){
    // get amount of songs in db    
    sqlite3 *db = openDB(DB_PATH);
    sqlite3_stmt *sql;
    char *query = COUNT_PLAYLIST_RELATIONS;
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to query playlist realation size:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }

	sqlite3_bind_text(sql, 1, playlistUuid, -1, NULL);

    // load song limit
    int relationLimit;
    result = sqlite3_step(sql);
    if(result == SQLITE_ROW){
        const char *columnValue = sqlite3_column_text(sql, 0);
        sscanf(columnValue, "%d", &relationLimit); // Using sscanf
    }

    sqlite3_finalize(sql);
    sqlite3_close(db);

    return relationLimit;
}


int addSongToPlaylist(char *playlistUuid, char *songUuid){

    // open db  
    sqlite3 *db = openDB(DB_PATH);
    // prepare statement    
    char *errMsg = 0;
    sqlite3_stmt *sql;
    int result = sqlite3_prepare_v2(db, ADD_SONG_PLAYLIST, -1, &sql, NULL);

    // check for sql cursor errors
    if(result != SQLITE_OK){
        fprintf(stderr, "Failed to insert:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return FALSE;
    }
	// generate uuid
	uuid_t relation_uuid_str; 
    char relation_uuid[37];
    uuid_generate_time_safe(relation_uuid_str);
    uuid_unparse_lower(relation_uuid_str, relation_uuid);
	// bind values
    sqlite3_bind_text(sql, 1, relation_uuid, -1, NULL);	
    sqlite3_bind_text(sql, 2, playlistUuid, -1, NULL);
    sqlite3_bind_text(sql, 3, songUuid, -1, NULL);

    // do first insert
    sqlite3_step(sql);
    sqlite3_close(db);

    return TRUE;
}


song_t **loadPlaylistSongs(char *playlistUuid){

	int limit = getRelationTableSize(playlistUuid); 
	song_t **songs = malloc(limit * sizeof(song_t*));
    // allocate space
    for(int i = 0; i < limit; i++){
        songs[i] = malloc(sizeof(song_t));
    }

	// open db
    sqlite3 *db = openDB(DB_PATH);
    // prepare statement    
    sqlite3_stmt *sql; 
    char *query = VIEW_PLAYLIST_SONGS; 
    // prepare statement
    int result = sqlite3_prepare_v2(db, query, -1, &sql, NULL);

	if(result != SQLITE_OK){
        fprintf(stderr, "Failed to view songs:  %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
    }
	// bind playlist uuid to query	
    sqlite3_bind_text(sql, 1, playlistUuid, -1, NULL);

	int indexCount = 0;
	while((result = sqlite3_step(sql)) == SQLITE_ROW){
		const char *song_uuid = sqlite3_column_text(sql, 2);
		char test[37];  
		strcpy(test, song_uuid);
		song_t *mySong = viewSongById(test); 
		songs[indexCount] = mySong;  
		indexCount += 1;
	}

	sqlite3_close(db); 
	return songs; 
}


void viewPlaylistSongs(char *playlistUuid){
	
	int relationLimit = getRelationTableSize(playlistUuid); 
	song_t **songs = loadPlaylistSongs(playlistUuid); 
	song_t ***p = &songs; 

	playlist_t *playlist = viewPlaylistById(playlistUuid); 
	printf("Viewing songs from playlist: %s\n", playlist->name); 

	// print header
	printf("\n");
    printf("\e[0;31m");
    printf("%-45s %-25s\n", "UUID", "Name");
	generateBanner(80); 
	// view playlist songs
	for(int i = 0; i < relationLimit; i++){
		char song_uuid[37];
        uuid_unparse_lower((*p)[i]->songId, song_uuid);
        printf("%-45s %-24s\n", song_uuid ,(*p)[i]->name);
	}
	printf("\n"); 
	
}




