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
void testLoadPlaylistsArray(); 



// url library tests


void runAllTests(); 

#endif
