BIN=bin
CC=gcc
TARGET=helloring

OBJECT_FILES = main.o log.o 

all: $(OBJECT_FILES) 
	gcc -o $(TARGET) $(OBJECT_FILES) -lportaudio -lportsf -lm

log.o: ../include/logging/log.c ../include/logging/log.h
	gcc -c ../include/logging/log.c

main.o: main.c 
	gcc -c main.c

clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~
 
