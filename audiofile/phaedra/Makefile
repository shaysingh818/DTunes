BIN=bin
CC=gcc
TARGET=phaedra

OBJECT_FILES = main.o log.o phaedra.o

all: $(OBJECT_FILES) 
	gcc -o $(TARGET) $(OBJECT_FILES) -lportaudio  -lm

phaedra.o: phaedra.c phaedra.h
	gcc -c phaedra.c

log.o: logging/log.c logging/log.h
	gcc -c logging/log.c

main.o: main.c 
	gcc -c main.c

clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~
