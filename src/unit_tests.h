#include <stdio.h> 

#define MAX_LEN 128


#define TEST "test" 
#define TEST_HELP_MENU "test-help"

// test cli aliases
#define TEST_INSERT_PLAYLIST  "t-cp" 
#define TEST_LOAD_PLAYLISTS  "t-lp" 
#define TEST_VIEW_PLAYLISTS  "t-vp"
#define  TEST_DELETE_PLAYLIST  "t-dp" 
#define TEST_DELETE_PLAYLISTS  "t-dap" 

// test cli aliases
#define TEST_INSERT_SONG  "t-cs" 
#define TEST_LOAD_SONGS  "t-ls" 
#define TEST_VIEW_SONGS  "t-vs" 
#define TEST_DELETE_SONG  "t-ds" 
#define TEST_DELETE_SONGS  "t-delete-s" 

#define TEST_SYNC_SONGS  "t-sync" 
#define TEST_DOWNLOAD_YT  "t-download" 


// help menu
void testCaseHelpMenu(char *myarg);
void printBanner(FILE *fptr); 

// playlist tests
void testInsertPlaylist(char *myarg, char *myarg2);
void testLoadPlaylists(char *myarg);
void testViewPlaylists(char *myarg); 
void testDeletePlaylist(char *myarg, char *myarg2);
void testDeletePlaylists(char *myarg);
 
// song tests
void testInsertSong(char *myarg, char *myarg2); 
void testLoadSongs(char *myarg); 
void testViewSongs(char *myarg); 
void testDeleteSong(char *myarg, char *myarg2);
void testDeleteSongs(char *myarg);

// test sync songs
void syncAudioFiles(char *myarg); 
void testDownloadYTVideo(char *myarg, char *myarg2); 


// run all tests
void runAllTests(char *myarg);
