#include <portsf.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <math.h> 

#define FRAME_BLOCK_LEN 512

int main(int argc, char** argv){
	
	float buf[FRAME_BLOCK_LEN]; 
	
	int sfd; /* audio file descriptor */ 
	int opened = 0; 
	PSF_PROPS props; 
	
	long counter;
	long length; 
	long endpoint; 
	
}
