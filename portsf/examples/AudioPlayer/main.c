#include <stdio.h>
#include <ncurses.h>

void keyboardInput(char key){
	switch(key){
		case 'D':
			printf("Go forward in queue\n");
			break;  
		case 'A':
			printf("Go back\n"); 
			break; 
		case ' ':
			printf("Pause"); 
			break; 
		case 'P':
			// check if player is stopped
			printf("Play\n"); 
			break; 
	}
}


int main(int argc, char** argv){


	while(1){
		char ch = getchar();
		keyboardInput(ch); 
	}





}
