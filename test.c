#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <time.h> 
#include <string.h> 
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define DEBUG FALSE


int main(int argc, char* argv[]){
	
	const char *cmd = "gnome-terminal command=\" ./dtunes help\" ";
	system(cmd); 

	
	const char *cmd2 = "gnome-terminal command=\" ./dtunes help\" ";
	system(cmd2); 
	
}
