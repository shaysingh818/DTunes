#include "collection_test.h" 
#include "collection.h"

void testCreateCollection(){
	
	char *mytime = getCurrentTime();
    // create playlist instance
    collection_t *newCollection;
    newCollection = (collection_t*)malloc(sizeof(collection_t));
    strcpy(newCollection->name, "test-collection");
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


