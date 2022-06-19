BIN=bin
CC=gcc
TARGET=phaedra

OBJECT_FILES = main.o log.o phaedra.o phaedra_endpoints.o endpoints.o

all: $(OBJECT_FILES) 
	gcc -o $(TARGET) $(OBJECT_FILES) -lportaudio -lportsf  -lm

phaedra.o: phaedra.c phaedra.h
	gcc -c phaedra.c

log.o: logging/log.c logging/log.h
	gcc -c logging/log.c

db.o: db/db.c db/db.h
	gcc -c db/db.c 

phaedra_endpoints.o: phaedra_endpoints.c phaedra_endpoints.h
	gcc -c phaedra_endpoints.c

endpoints.o: endpoints/endpoints.c endpoints/endpoints.h
	gcc -c endpoints/endpoints.c

main.o: main.c 
	gcc -c main.c

clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~
