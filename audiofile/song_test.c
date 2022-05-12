#include "song_test.h"
#include "song.h"
#include "../db/db.h"


void testCreateAudioFile(){

    // change to back to src directory
	/**
    if(chdir("../../src") != 0){
        dlog("FAILED", "CHANGE TO TESTING DIRECTORY");
    } */
    char *mytime = getCurrentTime();
    // create playlist instance
    song_t *newSong;
    newSong = (song_t*)malloc(sizeof(song_t));
    strcpy(newSong->name, "test-song");
    strcpy(newSong->dateCreated, mytime);
    strcpy(newSong->filePath, "streaming-path");
    strcpy(newSong->subtitles, "subtitles");
    newSong->plays = 0;

    int insertDbResult = createSong(newSong);
    // check result of insert 
    if(insertDbResult){
        //printf("\033[0;32m")  
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "INSERT AUDIO FILE");
    }else{
        printf("\e[0;31m");
        dlog("FAILED", "INSERT AUDIO FILE");
    }

}


void testUpdateAudioFileByName(){
	
    char *mytime = getCurrentTime();
    // create playlist instance
    song_t *newSong;
    newSong = (song_t*)malloc(sizeof(song_t));
    strcpy(newSong->name, "test-update-song");
    strcpy(newSong->dateCreated, mytime);
    strcpy(newSong->filePath, "streaming-path");
    strcpy(newSong->subtitles, "subtitles");
    newSong->plays = 0;

	// generate uuid
    char song_uuid[37];
    uuid_generate_time_safe(newSong->songId);
    uuid_unparse_lower(newSong->songId, song_uuid);

    int insertDbResult = createSong(newSong);
    // check result of insert 
    if(!insertDbResult){
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "UPDATE AUDIO FILE NAME");
	}

	int dbResult = updateSongByName("test-update-song", "updated-song-name"); 
	if(dbResult){	
        printf("\033[0;32m"); // green for pass
		dlog("PASSED", "UPDATE AUDIO FILE NAME"); 
	}else{
        printf("\033[0;31m"); // red for fail
		dlog("FAILED", "UPDATE AUDIO FILE NAME"); 
	}

	
    int dpResult = deleteAllSongs();
    if(!dpResult){
        dlog("FAILED", "VIEW AUDIO FILE BY NAME");
    }

}



void testUpdateAudioFileById(){
	
    char *mytime = getCurrentTime();
    // create playlist instance
    song_t *newSong;
    newSong = (song_t*)malloc(sizeof(song_t));
    strcpy(newSong->name, "test-update-song"); // original
    strcpy(newSong->dateCreated, mytime);
    strcpy(newSong->filePath, "streaming-path");
    strcpy(newSong->subtitles, "subtitles");
    newSong->plays = 0;

	// generate uuid
    char song_uuid[37];
    uuid_generate_time_safe(newSong->songId);
    uuid_unparse_lower(newSong->songId, song_uuid);

    int insertDbResult = createSong(newSong);
    // check result of insert 
    if(!insertDbResult){
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "UPDATE AUDIO FILE NAME");
	}

	int dbResult = updateSongById(song_uuid, "updated-song-name"); // new
	if(dbResult){	
        printf("\033[0;32m"); // green for pass
		dlog("PASSED", "UPDATE AUDIO FILE NAME"); 
	}else{
        printf("\033[0;31m"); // red for fail
		dlog("FAILED", "UPDATE AUDIO FILE NAME"); 
	}
}


void testDeleteAudioFile(){

    int dbResult = deleteSongByName("test-song");
    if(dbResult){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE AUDIO FILE");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE AUDIO FILE");
    }
}



void testDeleteAllAudioFiles(){

    int dbResult = deleteAllSongs();
    if(!dbResult){
        printf("\033[0;31"); // red for fail
        dlog("FAILED", "DELETE ALL AUDIO FILES");
    }

    int songResult = getSongTableSize();
    if(songResult == 0){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE ALL AUDIO FILES");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE ALL AUDIO FILES");
    }
}


void testLoadAudioFiles(){

    char *mytime = getCurrentTime();
    // allocate space for three playlists
    song_t **songs = malloc(3 * sizeof(song_t*));
    for(int i = 0; i < 3; i++){
        songs[i] = malloc(sizeof(song_t));
    }

    strcpy(songs[0]->name, "test song 1");
    strcpy(songs[0]->dateCreated, mytime);
    strcpy(songs[0]->filePath, "test file path 1");
    strcpy(songs[0]->subtitles, "test subtitles 1");

	// generate uuid
    char song_uuid[37];
    uuid_generate_time_safe(songs[0]->songId);
    uuid_unparse_lower(songs[0]->songId, song_uuid);


    strcpy(songs[1]->name, "test song 2");
    strcpy(songs[1]->dateCreated, mytime);
    strcpy(songs[1]->filePath, "test file path 2");
    strcpy(songs[1]->subtitles, "test subtitles 2");

	// generate uuid
    char song_uuid1[37];
    uuid_generate_time_safe(songs[1]->songId);
    uuid_unparse_lower(songs[1]->songId, song_uuid1);

    strcpy(songs[2]->name, "test song 3");
    strcpy(songs[2]->dateCreated, mytime);
    strcpy(songs[2]->filePath, "test file path 3");
    strcpy(songs[2]->subtitles, "test subtitles 3");
	
	// generate uuid
    char song_uuid2[37];
    uuid_generate_time_safe(songs[2]->songId);
    uuid_unparse_lower(songs[2]->songId, song_uuid2);

    // loop and insert all of them
    for(int j = 0; j < 3; j++){
        int insertDbResult = createSong(songs[j]);
        // check result of insert 
        if(!insertDbResult){
            printf("\033[0;31m"); // green for pass
            dlog("FAILED", "LOAD AUDIO FILES");
        }
    }

    // testing what's in the database
    int songLimit = getSongTableSize();
    song_t **songsDB = initSongs(songLimit);
    song_t ***p = &songsDB;

    for(int i = 0; i < songLimit; i++){
        int checkName = strcmp(songs[i]->name, (*p)[i]->name);
        int checkDate = strcmp(songs[i]->dateCreated, (*p)[i]->dateCreated);
        int checkPath = strcmp(songs[i]->filePath, (*p)[i]->filePath);
        int checkSubtitles = strcmp(songs[i]->subtitles, (*p)[i]->subtitles);

        int firstCondition = checkName == 0 && checkDate == 0;
        int secondCondition = checkPath == 0 && checkSubtitles == 0;

        if(firstCondition && secondCondition){
            printf("\033[0;32m"); // green for pass
            dlog("PASSED", "LOAD AUDIO FILES");
        }else{
            printf("\033[0;31m"); // red for fail
            dlog("FAILED", "LOAD AUDIO FILES");
        }

    }

    int dpResult = deleteAllSongs();
    if(!dpResult){
        dlog("FAILED", "LOAD AUDIO FILES");
    }

}


void testViewSongByName(){
	
    char *mytime = getCurrentTime();
    // create playlist instance
    song_t *newSong;
    newSong = (song_t*)malloc(sizeof(song_t));
    strcpy(newSong->name, "view-song-by-name");
    strcpy(newSong->dateCreated, mytime);
    strcpy(newSong->filePath, "streaming-path");
    strcpy(newSong->subtitles, "subtitles");
    newSong->plays = 0;

	// generate uuid
    char song_uuid[37];
    uuid_generate_time_safe(newSong->songId);
    uuid_unparse_lower(newSong->songId, song_uuid);


    int insertDbResult = createSong(newSong);
    // check result of insert 
    if(!insertDbResult){
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "UPDATE AUDIO FILE NAME");
	}


	song_t *testSong = viewSongByName("view-song-by-name"); 
	int checkName = strcmp(testSong->name, newSong->name);
    int checkDate = strcmp(testSong->dateCreated, newSong->dateCreated);
    int checkPath = strcmp(testSong->filePath, newSong->filePath);
    int checkSubtitles = strcmp(testSong->subtitles, newSong->subtitles);

    int firstCondition = checkName == 0 && checkDate == 0;
    int secondCondition = checkPath == 0 && checkSubtitles == 0;

    if(firstCondition && secondCondition){
    	printf("\033[0;32m"); // green for pass
        dlog("PASSED", "VIEW SONG BY NAME");
   	}else{
		printf("\033[0;31m"); // red for fail
		dlog("FAILED", "VIEW SONG BY NAME");
	}

	
    int dpResult = deleteAllSongs();
    if(!dpResult){
        dlog("FAILED", "VIEW SONG BY NAME");
    }
}



void testViewSongById(){

	char *currentTime = getCurrentTime(); 	
	song_t *newSong;
    newSong = (song_t*)malloc(sizeof(song_t));
    printf("\033[0;32m");

    // set values
    strcpy(newSong->name, "view-song-by-id");
    strcpy(newSong->dateCreated, currentTime);
    strcpy(newSong->filePath, "streaming-path");
    strcpy(newSong->subtitles, "");
    newSong->plays = 0;

    // generate uuid
    char song_uuid[37];
    uuid_generate_time_safe(newSong->songId);
    uuid_unparse_lower(newSong->songId, song_uuid);

    int insertDbResult = createSong(newSong);
    // check result of insert 
    if(!insertDbResult){
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "UPDATE AUDIO FILE NAME");
	}


	song_t *testSong = viewSongById(song_uuid); 
	int checkName = strcmp(testSong->name, newSong->name);
    int checkDate = strcmp(testSong->dateCreated, newSong->dateCreated);
    int checkPath = strcmp(testSong->filePath, newSong->filePath);
    int checkSubtitles = strcmp(testSong->subtitles, newSong->subtitles);

    int firstCondition = checkName == 0 && checkDate == 0;
    int secondCondition = checkPath == 0 && checkSubtitles == 0;

    if(firstCondition && secondCondition){
    	printf("\033[0;32m"); // green for pass
        dlog("PASSED", "VIEW SONG BY UUID");
   	}else{
		printf("\033[0;31m"); // red for fail
		dlog("FAILED", "VIEW SONG BY UUID");
	}

	
    int dpResult = deleteAllSongs();
    if(!dpResult){
        dlog("FAILED", "VIEW SONG BY NAME");
    }
}



void testSyncAudioFilesPath(){
	int result = loadAudioFilesFromDirectory("data/audiofiles");
    if(result){
    	d_log("PASSED", "SYNC SONGS TO DIRECTORY");
    }else{
    	d_log("FAILED", "SYNC SONGS TO DIRECTORY");
    }
}


void runAudioFilesTest(){
	// testing audio file storage
    printf("\033[0;37m");
    dlog("AUDIO FILE LIBRARY", "Running tests for audio file library");
    testCreateAudioFile();
	testUpdateAudioFileByName(); 
	testUpdateAudioFileById(); 
    testDeleteAudioFile();
    testDeleteAllAudioFiles();
    testLoadAudioFiles();
	testViewSongByName(); 
	testViewSongById(); 
	testSyncAudioFilesPath();

}
