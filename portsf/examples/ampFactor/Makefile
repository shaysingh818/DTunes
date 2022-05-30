BIN=bin
CC=gcc
TARGET=ampfactor

OBJECT_FILES = main.o log.o lib.o

all: $(OBJECT_FILES) 
	gcc -o $(TARGET) $(OBJECT_FILES) -lportsf -lm

log.o: ../include/logging/log.c ../include/logging/log.h
	gcc -c ../include/logging/log.c

lib.o: ../include/lib/lib.c ../include/lib/lib.h
	gcc -c ../include/lib/lib.c

main.o: main.c 
	gcc -c main.c

clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~
 
