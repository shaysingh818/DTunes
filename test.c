#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


struct Corm {
	char dbPath[100]; 
}; 

typedef struct Corm corm_t; 


struct Song {
	corm_t* orm; 
	char name[100];
	char dateCreated[100]; 
}; 

typedef struct Song song_t; 


void generateInsertQuery(song_t *model){
	
}

// set db path for instance of orm
corm_t *initCorm(char *dbPath){
	corm_t *newCorm = (corm_t*)malloc(sizeof(corm_t)); 
	strcpy(newCorm->dbPath, dbPath); 
	return newCorm; 
}

// open db and return instance of sqlite3
sqlite3* openDB(corm_t* cormInstance){
	sqlite3 *db; 
    if(sqlite3_open(cormInstance->dbPath, &db)){
        printf("Could not open the db : \n ");
         
        printf("Could not open the db:  %s\n",sqlite3_errmsg(db));
        exit(-1); 
    }
    return db; 
}

// inherited method of create instance
void create(corm_t* cormInstance){
	sqlite3 *db = openDB(cormInstance); 
	
	
}





int main(int argc, char* argv[]){

	// create instance of my orm
	corm_t *test = initCorm("data/db/dtunes.db"); 
}
