# MAKE FILE FOR DTUNES
# SUB MODULES ARE THE OBJECTS VARIABLES
# LOADER/DRIVER FILES are the main header files needed


PHAEDRA_OBJECTS= phaedra.o phaedra_endpoints.o phaedra_test.o
PHAEDRA_HEADER = phaedra/phaedra_endpoints.h
ENDPOINTS_OBJECTS = endpoints.o log.o
MAIN_HEADER_FILES = endpoints/endpoints.h collection/collection_endpoints.h
COLLECTION_OBJECTS = collection.o audio_file.o collection_test.o collection_endpoints.o
OBJECT_FILES = $(PHAEDRA_OBJECTS) $(ENDPOINTS_OBJECTS) $(COLLECTION_OBJECTS) $(KEYBOARD_OBJECTS)

BIN=bin
CC=gcc
TARGET=dtunes

FLAGS = -lportaudio -lportsf -lm -lsqlite3 -lpthread -luuid

all: main.o $(OBJECT_FILES)
	gcc -o $(TARGET) main.o $(OBJECT_FILES) $(FLAGS)

log.o: logging/log.c logging/log.h
	gcc -c logging/log.c

endpoints.o: endpoints/endpoints.c endpoints/endpoints.h
	gcc -c endpoints/endpoints.c

phaedra.o: phaedra/phaedra.c phaedra/phaedra.h
	gcc -c phaedra/phaedra.c

phaedra_test.o: phaedra/phaedra_test.c phaedra/phaedra_test.h
	gcc -c phaedra/phaedra_test.c

phaedra_endpoints.o: phaedra/phaedra_endpoints.c phaedra/phaedra_endpoints.h
	gcc -c phaedra/phaedra_endpoints.c

collection.o: collection/collection.c collection/collection.h
	gcc -c collection/collection.c

audio_file.o: collection/audio_file.c collection/audio_file.h
	gcc -c collection/audio_file.c

collection_test.o: collection/collection_test.c collection/collection_test.h
	gcc -c collection/collection_test.c

collection_endpoints.o: collection/collection_endpoints.c collection/collection_endpoints.h
	gcc -c collection/collection_endpoints.c

main.o: main.c $(MAIN_HEADER_FILES) $(PHAEDRA_HEADER)
	gcc -c main.c

clean_binary:	
	rm $(TARGET)

clean:
	rm -f *.o *~

