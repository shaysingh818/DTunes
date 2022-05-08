#ifndef TEST_H
#define TEST_H

// database library tests
void testOpenDb();
void testCurrentTime(); 
void testCombineFileStrs(); 
void testRemoveChar(); 
void testRenameFile(); 
void testCountFilesDirectory(); 
void testClearDirectory(); 

// audio file library tests
void testCreateAudioFile(); 
void testDeleteAudioFile();
void testDeleteAllAudioFiles();  
void testLoadAudioFiles(); 

// playlist library tests
void testCreatePlaylistCollection(); 
void testDeletePlaylistCollection();
void testDeleteAllPlaylistCollections();  
void testDeleteCascadingPlaylists(); 
void testLoadPlaylistsArray(); 
void testAddSongToPlaylist(); 

// url library tests
void testCreateUrl(); 
void testDeleteUrl();
void testDeleteAllUrls();  
void testLoadUrls();
void testBuildArgumentThreads(); 
void testAllocateSubArray(); 
void testGrabDatabaseUrls();  

// run all test functions above
void runAllTests(); 

#endif
