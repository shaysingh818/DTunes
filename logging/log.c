#include "log.h"

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


void generateBanner(int amount){
    char *str = malloc(amount * sizeof(char *));
    for(int i = 0; i < amount; i++){
        strcat(str, "=");
    }
    printf("%s\n", str);
    free(str);
}

