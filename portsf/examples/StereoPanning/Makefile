BIN=bin
CC=gcc
TARGET=stereoPanning

OBJECT_FILES = main.o log.o lib.o

all: $(OBJECT_FILES) 
	gcc -o $(TARGET) $(OBJECT_FILES) -lportsf -lm

log.o: logging/log.c logging/log.h
	gcc -c logging/log.c

lib.o: lib/lib.c lib/lib.h
	gcc -c lib/lib.c

main.o: main.c 
	gcc -c main.c  
