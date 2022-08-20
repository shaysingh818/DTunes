#ifndef LOG_H
#define LOG_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* getCurrentTime();
void d_log(char* type, char *message);
void dlog(char *type, char*message);
void dlog_int(char *type, int message);
void dlog_float(char *type, float* messsage);
void dlog_f(char *type, float messsage);
void dlog_counter(char *type, char *message, int counter);
void d_log_time(char* type, char* message);

#endif

