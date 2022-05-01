#include "db.h"
#include "song.h"
#include "playlist.h"
#include <uuid/uuid.h> 

int main(int argc, char **argv){

	dlog("AUDIO COLLECTION", "Testing audio collections feature");
	char *mytime = getCurrentTime();

    // create instance of playlist model    
    song_t *newSong;
    newSong = (song_t*)malloc(sizeof(song_t));
    printf("\033[0;32m");
    printf("[STRUCTURE ALLOCATION]: CREATED INSTANCE OF SONG:\n");

    // set values
    strcpy(newSong->name, "test name");
    strcpy(newSong->dateCreated, mytime);
    strcpy(newSong->filePath, "test-path");
    strcpy(newSong->subtitles, "test-titles");
    newSong->plays = 0;

	int dbResult = createSong(newSong); 
	if(dbResult){
		dlog("DB INSERT", "Inserted song"); 
	}
	
 
}
