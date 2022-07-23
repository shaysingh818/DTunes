#include "collection_test.h"
#include "../logging/log.h" 
#include "collection.h"

void testCreateCollection(){
	
	char *mytime = getCurrentTime();
    // create playlist instance
    int insertDbResult = createCollection("test-collection");
    // check result of insert 
    if(insertDbResult){
        //printf("\033[0;32m")  
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "INSERT COLLECTION");
    }else{
        printf("\e[0;31m");
        dlog("FAILED", "INSERT COLLECTION");
    }

}


void testDeleteCollection(){

    int dbResult = deleteCollection("test-collection");
    if(dbResult){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE PLAYLIST COLLECTION");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE PLAYLIST COLLECTION");
    }
}


void testDeleteAllCollections(){

    int dbResult = deleteAllCollections();
    if(!dbResult){
        printf("\033[0;31"); // red for fail
        dlog("FAILED", "DELETE ALL PLAYLISTS");
    }

    int collectionResult = getCollectionTableSize();
    if(collectionResult == 0){
        printf("\033[0;32m"); // green for pass
        dlog("PASSED", "DELETE ALL PLAYLISTS");
    }else{
        printf("\033[0;31m"); // green for pass
        dlog("FAILED", "DELETE ALL PLAYLISTS");
    }
}


void testLoadCollections(char *path) {

	char **collectionNames;
	collectionNames = malloc(10 * sizeof(char*)); 

	for(int i = 0; i < 10; i++){
		collectionNames[i] = malloc(100 * sizeof(char)); 	
		char formatName[100]; 
		sprintf(formatName, "test-%d", i);
		strcpy(collectionNames[i], formatName); 
	}

	// create 10 fake collections
	for(int k = 0; k < 10; k++){

    	int insertDbResult = createCollection(collectionNames[k]);
	
    	if(!insertDbResult){
        	printf("\e[0;31m");
        	dlog("FAILED", "INSERT COLLECTION");
    	}

    	if(chdir(path) != 0){
			dlog("ERROR", "chirdir () to current dir failed"); 
    	}
	}

	// testing what's in the database
	int collectionLimit = getCollectionTableSize();
    collection_t **collectionDB = initCollections(collectionLimit);
    collection_t ***p = &collectionDB;

	
	for(int j = 0; j < collectionLimit; j++){
        int checkName = strcmp(collectionNames[j], (*p)[j]->name);

        if(checkName != 0){
            printf("\033[0;31m"); // green for pass
            dlog("FAILED", "LOAD COLLECTION");
		}

    }

    int dbResult = deleteAllCollections();
    if(!dbResult){
        printf("\033[0;31"); // red for fail
        dlog("FAILED", "DELETE ALL COLLECTIONS LOADED");
    }


	printf("\033[0;32m"); // green for pass
	dlog("PASSED", "LOAD COLLECTION");

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

    folder = opendir("adms/testing");
     if(folder == NULL){
        perror("Unable to read directory\n");
    }


    // change to desired directory
    if(chdir("adms/testing") != 0){
        dlog("ERROR", "CHANGE DIR");
    }

    system("touch testingfile.txt");
    int result = renameFile("testingfile.txt", "newfile.txt");

	if(result == FALSE){
		dlog("FAILED", "RENAME FILE"); 
	}

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




void runCollectionTests() {

	char path[200]; 
	if (getcwd(path, 200) == NULL) {
        if (errno == ERANGE){
			printf("[ERROR] pathname length exceeds the buffer size\n");
		}else{
            perror("getcwd");
		}
    }

	testCreateCollection();

	 // delete folder of songs
    if(chdir(path) != 0){
		dlog("ERROR", "chirdir () to current dir failed"); 
    }
 
	testDeleteCollection(); 	
	 // delete folder of songs
    if(chdir(path) != 0){
		dlog("ERROR", "chirdir () to current dir failed"); 
    }

	testLoadCollections(path);
 
    if(chdir(path) != 0){
		dlog("ERROR", "chirdir () to current dir failed"); 
    }

	testDeleteAllCollections();


	/* run file processing tests */  
	testCombineFileStrs();
	testRemoveChar();
	testRenameFile();
	testCountFilesDirectory();
	

}

