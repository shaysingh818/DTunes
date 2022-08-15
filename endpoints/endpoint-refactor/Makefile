OBJECTS= endpoints.o
MAIN_HEADER_FILES = endpoints.h

BIN=bin
CC=gcc
TARGET=endpnt_refactor

FLAGS = -lm

all: main.o $(OBJECTS)
	gcc -o $(TARGET) main.o $(OBJECTS) $(FLAGS)

endpoints.o: endpoints.c endpoints.h
	gcc -c endpoints.c

main.o: main.c $(MAIN_HEADER_FILES) 
	gcc -c main.c

clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~


