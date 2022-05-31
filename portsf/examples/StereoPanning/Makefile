BIN=bin
CC=gcc
TARGET=sfpan

OBJECT_FILES = main.o log.o wrapper.o

all: $(OBJECT_FILES) 
	gcc -o $(TARGET) $(OBJECT_FILES) -lportsf -lm

log.o: ../include/logging/log.c ../include/logging/log.h
	gcc -c ../include/logging/log.c

wrapper.o: ../include/wrapper/wrapper.c ../include/wrapper/wrapper.h
	gcc -c ../include/wrapper/wrapper.c

main.o: main.c 
	gcc -c main.c


clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~
 
