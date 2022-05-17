#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void d_log(char* type, char *message); 
void dlog(char *type, char*message); 
void dlog_int(char *type, int message); 
void dlog_counter(char *type, char *message, int counter); 
void d_log_time(char* type, char* message); 
void generateBanner(int amount);

#endif
