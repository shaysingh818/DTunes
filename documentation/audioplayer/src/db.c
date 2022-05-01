#include "db.h"

sqlite3* openDB(char *filename){	
	sqlite3 *db; 
	if(sqlite3_open(filename, &db)){
		printf("Could not open the db : \n ");
		 
        printf("Could not open the db:  %s\n",sqlite3_errmsg(db));
		exit(-1); 
	}
	return db; 
}


// Get current time for date timestamp on create/update
char* getCurrentTime(){
	time_t rawtime; 
	struct tm * timeinfo; 
	// get time
	time(&rawtime);
	timeinfo = localtime(&rawtime); 
	// return current time
	return asctime(timeinfo);
}


void d_log(char* type, char* message){
	char* currTime = getCurrentTime() + '\0'; 
	printf("[%s]: %s\n",  type, message);
}


// logging functions
void dlog(char *type, char *message){
    printf("[%s]: %s\n", type, message);
}

// logging functions
void dlog_int(char *type, int message){
    printf("[%s]: %d\n", type, message);
}

void dlog_counter(char *type, char *message, int counter){
	printf("[%d]: %s  %s\n", counter, type, message); 
}


void d_log_time(char* type, char* message){ 
	time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    printf("[%s][%s]: %s\n",  time_str, type, message);
}


int downloadVideo(char *url){
    char buffer[500];
    if(chdir(YOUTUBE_FILE_PATH) != 0){
        dlog("ERROR", "CHANGE DIR YOUTUBE");
		perror("chdir() to /error failed");  
        return FALSE;
    } 
    sprintf(buffer, "python3 yt.py %s", url);
    dlog("COMMAND", buffer);
    system(buffer);
    return TRUE;
}


void backupVideo(char *dbUrl){
    char buffer[500];
    sprintf(buffer, "python3 yt.py %s", dbUrl);
    dlog("COMMAND", buffer);
    system(buffer);
}



char* combineFileStrs(const char *cwd, const char *fileName){
	char *result = malloc(strlen(cwd) + strlen(fileName) + 1); 
	strcpy(result, cwd);
	strcat(result, fileName); 
	return result;  
}



void removeChar(char *testString, char charToRemove){	
    char *d = testString;
    do {
        while (*d == charToRemove){
            ++d;
        }
    } while(*testString++ = *d++);
}


void getCurrentDirectory(char *cwd){
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        dlog("ERROR", "CURRENT WORKING DIRECTORY");
    }else{
        if(DEBUG == TRUE){
            dlog("CURRENT DIR", cwd);
        }
    }
}


void renameFile(char *fileName, char *newFileName){
    int result = rename(fileName, newFileName);
    if(result == 0){
        if(DEBUG == TRUE){
            d_log("FILE", "RENAMED");
        }
    }else{
        d_log("FILE", "RENAME ERROR");
    }
}


// function to count amount of files in directory
int countFiles(char *directoryPath){

	int file_count = 0;
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(directoryPath); /* There should be error handling after this */
	while ((entry = readdir(dirp)) != NULL) {
    	if (entry->d_type == DT_REG) { /* If the entry is a regular file */
         file_count++;
    	}
	}
	closedir(dirp);
	return file_count; 
}



void reformatFileNames(char *desiredPath){
	/**		
		desiredPath: Desired file directory that you want to preprocess

		Purpose: Goes into a file directory and reformats all the file names with 
		NO Spaces. removing spaces is needed to create a single character pointer for 
		the streaming path. 
	*/
	
 	DIR *folder;
    struct dirent *entry;
    int files = 0; 
    folder = opendir(desiredPath);
    if(folder == NULL){
    	perror("Unable to read directory\n");
    } 
	
	// change to desired directory
	if(chdir(AUDIO_FILE_PATH) != 0){	
		dlog("ERROR", "CHANGE DIR"); 
	}

    while((entry=readdir(folder))){	
		files++;
		// file information
		char *tempFile = entry->d_name; 
		char *tempFileName = entry->d_name;
		char *currTime = getCurrentTime(); 
	
		// get current working directory
		char cwd[256]; 
		if(getcwd(cwd, sizeof(cwd)) == NULL){
			dlog("ERROR", "CURRENT WORKING DIRECTORY"); 
		}

		// get previous file name 
		strcat(cwd, "/"); 
		char *idk = malloc(strlen(tempFileName) + 1); 
		strcpy(idk, tempFileName); 

		// remove all spaces in files
		removeChar(tempFileName, ' ');	
		removeChar(tempFileName, ',');
		removeChar(tempFileName, '\'');	
	
		renameFile(idk, tempFileName); 

		dlog_counter("REFORMAT FILE", idk, files); 
	}

	closedir(folder);
}


void clearAudioFileDirectory(char *desiredPath){	
	DIR *audioFolder = opendir(desiredPath);
	struct dirent *next_file; 
	char filepath[300];

	while((next_file=readdir(audioFolder)) != NULL){
		sprintf(filepath, "%s/%s", desiredPath, next_file->d_name); 
		remove(filepath); 
	}
	closedir(audioFolder); 
}
