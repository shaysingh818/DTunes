#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h> 
#include <linux/input.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define INPUT_DIR "/dev/input/"
#define NUM_EVENTS 128
#define NUM_KEYCODES 71


const char *keycodes[] = {
    "RESERVED",
    "ESC",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "MINUS",
    "EQUAL",
    "BACKSPACE",
    "TAB",
    "Q",
    "W",
    "E",
    "R",
    "T",
    "Y",
    "U",
    "I",
    "O",
    "P",
    "LEFTBRACE",
    "RIGHTBRACE",
    "ENTER",
    "LEFTCTRL",
    "A",
    "S",
    "D",
    "F",
    "G",
    "H",
    "J",
    "K",
    "L",
    "SEMICOLON",
    "APOSTROPHE",
    "GRAVE",
    "LEFTSHIFT",
    "BACKSLASH",
    "Z",
    "X",
    "C",
    "V",
    "B",
    "N",
    "M",
    "COMMA",
    "DOT",
    "SLASH",
    "RIGHTSHIFT",
    "KPASTERISK",
    "LEFTALT",
    "SPACE",
    "CAPSLOCK",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "NUMLOCK",
    "SCROLLLOCK"
};


static int is_char_device(const struct dirent *file){
    struct stat filestat;
    char filename[512];
    int err;

    snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, file->d_name);

    err = stat(filename, &filestat);
    if(err){
        return 0;
    }

    return S_ISCHR(filestat.st_mode);
}

char *get_keyboard_event_file(void){
    char *keyboard_file = NULL;
    int num, i;
    struct dirent **event_files;
    char filename[512];

    num = scandir(INPUT_DIR, &event_files, &is_char_device, &alphasort);
    if(num < 0){
        return NULL;
    }


    else {
        for(i = 0; i < num; ++i){
            int32_t event_bitmap = 0;
            int fd;
            int32_t kbd_bitmap = KEY_A | KEY_B | KEY_C | KEY_Z; 
		
			printf("File: %s\n", filename); 

            snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, event_files[i]->d_name);
            fd = open(filename, O_RDONLY);

            if(fd == -1){
                perror("open");
                continue;
            }

            ioctl(fd, EVIOCGBIT(0, sizeof(event_bitmap)), &event_bitmap);
            if((EV_KEY & event_bitmap) == EV_KEY){
                // The device acts like a keyboard

                ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(event_bitmap)), &event_bitmap);
                if((kbd_bitmap & event_bitmap) == kbd_bitmap){
                    // The device supports A, B, C, Z keys, so it probably is a keyboard
                    keyboard_file = strdup(filename);
                    close(fd);
                    break;
                }
            }
            close(fd);

        }
    } 
    
    // Cleanup scandir
    for(i = 0; i < num; ++i){
        free(event_files[i]);
    }

    free(event_files);
    return keyboard_file;

}


void keylogger(int fd, struct input_event ev, ssize_t n){
	 
	while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            break;
    	}

		if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2){

			switch(*keycodes[ev.code]){

				case 'S':
					printf("Pressed Pause\n"); 
					break;
 
				case 'P':
					printf("Pressed Play\n"); 
					break; 
	
				case 'R':
					printf("Pressed Resume\n"); 
					break; 
				
			}
			 
		}

	}
	

}

int main(int argc, char** argv){


	struct input_event ev; 
	ssize_t n; 
	int fd; 
	char *keyboard_device = get_keyboard_event_file();
	int keyboard; 

	/** 
	if(!keyboard_device){
		exit(0); 
	}*/

	printf("device: %s\n", keyboard_device); 

	fd = open(keyboard_device, O_RDONLY); 
	if(fd == -1){
		printf("Could not open %s\n", keyboard_device); 
	}


	keylogger(fd, ev, n); 

	//keylogger(keyboard); 

	



}
