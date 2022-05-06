#include "test.h"
#include "playlist.h"
#include "song.h"
#include "urls.h"
#include "db.h"

/**
	INTERNAL TESTING SUITE FOR DTUNES
	This tests every single cli command
*/


void testOpenDb(){
	sqlite3 *testdb = openDB(DB_PATH); 
	if(testdb){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "OPEN DB METHOD"); 
	}else{
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "OPEN DB METHOD"); 
	}
}

void testCurrentTime(){
	char *currentTime = getCurrentTime(); 
	// time value two
	time_t rawtime;
    struct tm * timeinfo;
    // get time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

	char *testTime = asctime(timeinfo); 
	int result = strcmp(currentTime, testTime); 
	if(result == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "GET CURRENT TIME"); 
	}else{	
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "GET CURRENT TIME"); 
	}
}


void testCombineFileStrs(){

	char testStr[10] = "hello"; 
	char testStr2[10] = "world"; 
	char restStr[20] = "helloworld"; 
	char *testMethod = combineFileStrs(testStr, testStr2); 
	 
	int result = strcmp(testMethod, restStr); 
	if(result == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "COMBINE STR METHOD"); 
	}else{	
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "COMBINE STR METHOD"); 
	}

}


void testRemoveChar(){

	char testStr[10] = "hello";
	char resultStr[10] = "hell";  
	removeChar(testStr, 'o'); 
	
	int result = strcmp(testStr, resultStr); 
	if(result == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "REMOVE CHARACTER"); 
	}else{	
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "REMOVE CHARACTER"); 
	}
}



void testRenameFile(){

	DIR *folder;
    struct dirent *entry;
    int files = 0;

    folder = opendir("../data/testing");
	 if(folder == NULL){
        perror("Unable to read directory\n");
    }


	// change to desired directory
    if(chdir("../data/testing") != 0){
        dlog("ERROR", "CHANGE DIR");
    }

	system("touch testingfile.txt"); 
	renameFile("testingfile.txt", "newfile.txt"); 

	while((entry=readdir(folder))){
        files++;
        char *tempFile = entry->d_name;

		
		if(strcmp(tempFile, "..") == 0 || strcmp(tempFile, ".") == 0){
			continue; 
		}else{

			int result = strcmp(tempFile, "newfile.txt");

			if(result == 0){
				printf("\033[0;32m"); // green for pass
				dlog("PASSED", "RENAME FILE"); 
			}else{	
				printf("\e[0;31m"); // red for fail
				dlog("FAILED", "RENAME FILE"); 
			}
		}
			
    }

    closedir(folder);
	system("rm newfile.txt"); 
}



void testCountFilesDirectory(){
	
	DIR *folder;
    struct dirent *entry;
    int files = 0;


	system("mkdir test_folder"); 
	
	// check if we can view the test folder	
    folder = opendir("test_folder/");
	 if(folder == NULL){
        perror("Unable to read directory\n");
    }

	
	// change to desired directory
    if(chdir("test_folder/") != 0){
        dlog("FAILED", "CHANGE TO TESTING DIRECTORY");
    }

	
	system("touch test1.txt"); 
	system("touch tes21.txt"); 
	system("touch test2.txt");

	
	// change to desired directory
    if(chdir("../") != 0){
        dlog("FAILED", "CHANGE TO TESTING DIRECTORY");
    }

	
	int fileCount = countFiles("test_folder/");
	if(fileCount == 3){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "DIRECTORY FILE COUNTER"); 
	}else{
		printf("\e[0;31m"); // red for fail
		dlog("FAILED", "DIRECTORY FILE COUNTER"); 
	}


	system("rm -r test_folder"); 
}



void testCreateAudioFile(){
	
	// change to back to src directory
    if(chdir("../../src") != 0){
        dlog("FAILED", "CHANGE TO TESTING DIRECTORY");
    }
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


void testDeleteAudioFile(){

	int dbResult = deleteSong("test-song");
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
	 
	strcpy(songs[1]->name, "test song 2"); 	
	strcpy(songs[1]->dateCreated, mytime); 	
	strcpy(songs[1]->filePath, "test file path 2");
	strcpy(songs[1]->subtitles, "test subtitles 2");
		
	strcpy(songs[2]->name, "test song 3"); 	
	strcpy(songs[2]->dateCreated, mytime); 
	strcpy(songs[2]->filePath, "test file path 3");
	strcpy(songs[2]->subtitles, "test subtitles 3");

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


void testCreatePlaylistCollection(){
	
	// change to back to src directory
    //if(chdir("../../src") != 0){
    //    dlog("FAILED", "CHANGE TO TESTING DIRECTORY");
    //}

	char *mytime = getCurrentTime(); 
	// create playlist instance
	playlist_t *newPlaylist; 
	newPlaylist = (playlist_t*)malloc(sizeof(playlist_t)); 
	strcpy(newPlaylist->name, "test-playlist"); 
	strcpy(newPlaylist->dateCreated, mytime); 
	int insertDbResult = createPlaylist(newPlaylist);
    // check result of insert 
    if(insertDbResult){
        //printf("\033[0;32m")	
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "INSERT PLAYLIST COLLECTION"); 
    }else{
        printf("\e[0;31m");	
		dlog("FAILED", "INSERT PLAYLIST COLLECTION"); 
    }

}

void testDeletePlaylistCollection(){

	int dbResult = deletePlaylistByName("test-playlist");
    if(dbResult){	
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "DELETE PLAYLIST COLLECTION"); 
    }else{	
		printf("\033[0;31m"); // green for pass
		dlog("FAILED", "DELETE PLAYLIST COLLECTION"); 
    }
}


void testDeleteAllPlaylistCollections(){

	int dbResult = deleteAllPlaylists();
    if(!dbResult){	
		printf("\033[0;31"); // red for fail
		dlog("FAILED", "DELETE ALL PLAYLISTS"); 
    }

	int playlistResult = getPlaylistTableSize(); 
	if(playlistResult == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "DELETE ALL PLAYLISTS");
	}else{
		printf("\033[0;31m"); // green for pass
		dlog("FAILED", "DELETE ALL PLAYLISTS");
	} 
}


void testLoadPlaylistsArray(){

	char *mytime = getCurrentTime(); 
	// allocate space for three playlists
	playlist_t **playlists = malloc(3 * sizeof(playlist_t*));
	for(int i = 0; i < 3; i++){
		playlists[i] = malloc(sizeof(playlist_t));  
	}

	strcpy(playlists[0]->name, "testing 1"); 	
	strcpy(playlists[0]->dateCreated, mytime);
	 
	strcpy(playlists[1]->name, "testing 2"); 	
	strcpy(playlists[1]->dateCreated, mytime); 
		
	strcpy(playlists[2]->name, "testing 3"); 	
	strcpy(playlists[2]->dateCreated, mytime); 

	// loop and insert all of them
	for(int j = 0; j < 3; j++){
		int insertDbResult = createPlaylist(playlists[j]);
    	// check result of insert 
    	if(!insertDbResult){
			printf("\033[0;31m"); // green for pass
			dlog("FAILED", "LOAD PLAYLIST TEST CASE");
		} 
	}

	// testing what's in the database
	int playlistLimit = getPlaylistTableSize(); 
	playlist_t **playlistsDB = initPlaylists(playlistLimit); 
	playlist_t ***p = &playlistsDB; 

	for(int i = 0; i < playlistLimit; i++){
		int checkName = strcmp(playlists[i]->name, (*p)[i]->name);	
		int checkDate = strcmp(playlists[i]->dateCreated, (*p)[i]->dateCreated);

		if(checkName == 0 && checkDate == 0){			
			printf("\033[0;32m"); // green for pass
			dlog("PASSED", "LOAD PLAYLIST"); 
		}else{	
			printf("\033[0;31m"); // red for fail
			dlog("FAILED", "LOAD PLAYLIST"); 
		}

	}

	int dpResult = deleteAllPlaylists(); 
	if(!dpResult){
		dlog("FAILED", "DELETE PLAYLIST LOADED"); 
	}

}


void runAllTests(){

	// test database library
	dlog("DATABASE LIBRARY", "Running tests for database library"); 
	testOpenDb(); 
	testCurrentTime(); 
	testCombineFileStrs(); 
	testRemoveChar(); 
	testRenameFile(); 
	testCountFilesDirectory();

	 
	// testing audio file storage
	printf("\033[0;37m"); 	
	dlog("AUDIO FILE LIBRARY", "Running tests for audio file library"); 	
	testCreateAudioFile(); 
	testDeleteAudioFile();
	testDeleteAllAudioFiles();  
	testLoadAudioFiles(); 

	// testing playlists
	printf("\033[0;37m"); 	
	dlog("PLAYLIST LIBRARY", "Running tests for playlist library"); 	
	testCreatePlaylistCollection(); 
	testDeletePlaylistCollection(); 
	testDeleteAllPlaylistCollections(); 
	testLoadPlaylistsArray(); 
	// testing songs


	// testing urls


	// testing url downloader

}
