
CC=gcc
TARGET=breakpoint
OBJ_FILES=breakpoint.o log.o

all: $(OBJ_FILES)
	gcc -o $(TARGET) $(OBJ_FILES)

breakpoint.o: breakpoint.c breakpoint.h
	gcc -c breakpoint.c

log.o: log.c log.h
	gcc -c log.c
