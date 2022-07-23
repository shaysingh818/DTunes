#ifndef COLLECTION_TEST_H
#define COLLECTION_TEST_H


/* database function tests */
void testOpenDb();
void testCreateCollection(); 
void testDeleteCollection(); 
void testDeleteAllCollections(); 
void testLoadCollections(char *path);

/* file processing tests */
void testCombineFileStrs();
void testRemoveChar();
void testRenameFile();
void testCountFilesDirectory();
void testClearDirectory(); 
void runCollectionTests(); 

#endif
