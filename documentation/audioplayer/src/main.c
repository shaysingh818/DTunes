#include "db.h"
#include "song.h"
#include "playlist.h"
#include <uuid/uuid.h> 

int main(int argc, char **argv){

	dlog("AUDIO COLLECTION", "Testing audio collections feature");

	int result = viewSongs();
 	if(result){
		dlog("ACTION REPORT", "IT WORKS"); 
	}
	
 
}
