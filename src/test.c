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


void testAddSongToPlaylist(){

	// create playlist
	char *mytime = getCurrentTime(); 	
	playlist_t *newPlaylist; 
	newPlaylist = (playlist_t*)malloc(sizeof(playlist_t)); 
	strcpy(newPlaylist->name, "test-playlist"); 
	strcpy(newPlaylist->dateCreated, mytime); 
	int insertDbResult = createPlaylist(newPlaylist);
    // check result of insert 
    if(!insertDbResult){
        printf("\e[0;31m");	
		dlog("FAILED", "ADD SONG TO PLAYLIST"); 
    }

	// allocate space for three playlists
	song_t **songs = malloc(3 * sizeof(song_t*));
	for(int i = 0; i < 3; i++){
		songs[i] = malloc(sizeof(song_t));  
	}

	strcpy(songs[0]->name, "test song 1 (add to playlist) "); 	
	strcpy(songs[0]->dateCreated, mytime);	
	strcpy(songs[0]->filePath, "test file path 1");
	strcpy(songs[0]->subtitles, "test subtitles 1");
	 
	strcpy(songs[1]->name, "test song 2 (add to playlist) "); 	
	strcpy(songs[1]->dateCreated, mytime); 	
	strcpy(songs[1]->filePath, "test file path 2");
	strcpy(songs[1]->subtitles, "test subtitles 2");
		
	strcpy(songs[2]->name, "test song 3 (add to playlist) "); 	
	strcpy(songs[2]->dateCreated, mytime); 
	strcpy(songs[2]->filePath, "test file path 3");
	strcpy(songs[2]->subtitles, "test subtitles 3");

	// insert 3 songs
	for(int j = 0; j < 3; j++){
		int insertDbResult = createSong(songs[j]);
    	// check result of insert 
    	if(!insertDbResult){
			printf("\033[0;31m"); // green for pass
			dlog("FAILED", "ADD SONG TO PLAYLIST");
		} 
	}
	
	// get songs from the database
	int songLimit = getSongTableSize(); 
	song_t **songsDB = initSongs(songLimit); 
	song_t ***p = &songsDB;

	// view playlist
	playlist_t *playlist = viewPlaylistByName("test-playlist");  
	char playlist_uuid[37]; 
	uuid_unparse_lower(playlist->playlistId, playlist_uuid); 


	// add every song to the playlist
	for(int i = 0; i < songLimit; i++){
		char song_uuid[37]; 
		uuid_unparse_lower((*p)[i]->songId, song_uuid); 
		// insert song to playlist
		int addSong = addSongToPlaylist(playlist_uuid, song_uuid); 
		if(!addSong){
			dlog("FAILED", "ADD SONG TO PLAYLIST"); 
		}
    }


	// test if songs in playlist are similar to insert
	int relationLimit = getRelationTableSize(playlist_uuid);
    song_t **playlistSongs = loadPlaylistSongs(playlist_uuid);
    song_t ***pSongs = &playlistSongs;

	
	for(int j = 0; j < relationLimit; j++){
		int checkName = strcmp(playlistSongs[j]->name, (*pSongs)[j]->name);	
		int checkDate = strcmp(playlistSongs[j]->dateCreated, (*pSongs)[j]->dateCreated);	
		int checkPath = strcmp(playlistSongs[j]->filePath, (*pSongs)[j]->filePath);	
		int checkSubtitles = strcmp(songs[j]->subtitles, (*pSongs)[j]->subtitles);

		int firstCondition = checkName == 0 && checkDate == 0; 
		int secondCondition = checkPath == 0 && checkSubtitles == 0; 

		if(firstCondition && secondCondition){			
			printf("\033[0;32m"); // green for pass
			dlog("PASSED", "ADD SONG TO PLAYLIST"); 
		}else{	
			printf("\033[0;31m"); // red for fail
			dlog("FAILED", "ADD SONG TO PLAYLIST"); 
		}

	}

	int deleteSongs = deleteAllSongs(); 
	int deletePlaylists = deleteAllPlaylists(); 
	if(!deleteSongs){
		dlog("FAILED", "DELETE ALL SONGS (ADD TO PLAYLIST)"); 
	}

		
	if(!deletePlaylists){
		dlog("FAILED", "DELETE ALL PLAYLISTS (ADD TO PLAYLIST)"); 
	}	
	
}



void testCreateUrl(){

	
	char *mytime = getCurrentTime(); 
	url_t *newUrl;
    newUrl = (url_t*)malloc(sizeof(url_t));

    // set values
    strcpy(newUrl->url, "https://youtube.com"); // no url validation yet
    strcpy(newUrl->dateCreated, mytime);

    int dbResult = createYoutubeUrl(newUrl);
    if(dbResult){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "INSERT URL"); 
    }else{
		printf("\033[0;31m"); // red for fail
		dlog("FAILED", "INSERT URL"); 
    }

	
}


void testDeleteUrl(){

	int dbResult = deleteYoutubeUrl("https://youtube.com");
    if(dbResult){	
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "DELETE URL"); 
    }else{	
		printf("\033[0;31m"); // green for pass
		dlog("FAILED", "DELETE URL"); 
    }
}



void testDeleteAllUrls(){

	int dbResult = deleteAllYoutubeUrls();
    if(!dbResult){	
		printf("\033[0;31"); // red for fail
		dlog("FAILED", "DELETE ALL URLS"); 
    }

	int urlResult = getUrlTableSize(); 
	if(urlResult == 0){
		printf("\033[0;32m"); // green for pass
		dlog("PASSED", "DELETE ALL URLS");
	}else{
		printf("\033[0;31m"); // green for pass
		dlog("FAILED", "DELETE ALL URLS");
	} 
}




void testLoadUrls(){

	char *mytime = getCurrentTime(); 
	// allocate space for three playlists
	url_t **temp_urls = malloc(3 * sizeof(url_t*));
	for(int i = 0; i < 3; i++){
		temp_urls[i] = malloc(sizeof(url_t));  
	}

	strcpy(temp_urls[0]->url, "https://www.youtube.com"); 	
	strcpy(temp_urls[0]->dateCreated, mytime);
	 
	strcpy(temp_urls[1]->url, "https://www.google1.com"); 	
	strcpy(temp_urls[1]->dateCreated, mytime); 
		
	strcpy(temp_urls[2]->url, "https://www.youtubeone.com"); 	
	strcpy(temp_urls[2]->dateCreated, mytime); 

	// loop and insert all of them
	for(int j = 0; j < 3; j++){
		int insertDbResult = createYoutubeUrl(temp_urls[j]);
    	// check result of insert 
    	if(!insertDbResult){
			printf("\033[0;31m"); // green for pass
			dlog("FAILED", "LOAD URLS TEST CASE");
		} 
	}


	// Call the load songs with limit   
    int urlLimit = getUrlTableSize();
    url_t **urls = initUrls(urlLimit);
    url_t ***p = &urls;

    for(int i = 0; i < urlLimit; i++){
		int checkUrl = strcmp(temp_urls[i]->url, (*p)[i]->url); 
		int checkDate = strcmp(temp_urls[i]->dateCreated, (*p)[i]->dateCreated);
		
		if(checkUrl == 0 && checkDate == 0){			
			printf("\033[0;32m"); // green for pass
			dlog("PASSED", "LOAD URLS"); 
		}else{	
			printf("\033[0;31m"); // red for fail
			dlog("FAILED", "LOAD URLS"); 
		}
    }


	int dpResult = deleteAllYoutubeUrls(); 
	if(!dpResult){
		dlog("FAILED", "LOAD URLS (DELETE) "); 
	}

}



void testBuildArgumentThreads(){

	// insert some dummy urls
	char *mytime = getCurrentTime();
    // allocate space for three playlists
    url_t **temp_urls = malloc(6 * sizeof(url_t*));
    for(int i = 0; i < 6; i++){
        temp_urls[i] = malloc(sizeof(url_t));
    }
	
	char tempUrl[40] = "https://www.youtube.com"; 
	for(int i = 0; i < 6; i++){

		//char intToStr[] = i + '0'; 
		//strcat(tempUrl, intToStr); 

    	strcpy(temp_urls[i]->url, tempUrl);
    	strcpy(temp_urls[i]->dateCreated, mytime);
	}

	  // loop and insert all of them
    for(int j = 0; j < 6; j++){
        int insertDbResult = createYoutubeUrl(temp_urls[j]);
        // check result of insert 
        if(!insertDbResult){
            printf("\033[0;31m"); // green for pass
            dlog("FAILED", "BUILD ARGUMENT THREADS");
        }
    }



	/**
	int threadCount = 2; // split by 2 threads (without remainder)
	thread_arg *head = NULL;
    int urlLimit = getUrlTableSize();
    url_t **urls = initUrls(urlLimit);
    url_t ***p = &urls;

	int start, split, end, indexCount; 
	for(int i = 0; i < urlLimit; i += threadCount){
		start = i; 
		split = i + threadCount; 
		end = urlLimit - 1; 
		
	}

	*/

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
	testAddSongToPlaylist();  

	// testing urls	
	printf("\033[0;37m"); 	
	dlog("URL DOWNLOAD LIBRARY", "Running tests for url downloading library"); 	
	testCreateUrl(); 
	testDeleteUrl(); 
	testDeleteAllUrls(); 
	testLoadUrls(); 
	testBuildArgumentThreads(); 

	// testing url downloader

}
