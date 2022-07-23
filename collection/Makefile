ENDPNTS = endpoints.o log.o
COLLECTION_OBJECTS = collection.o collection_test.o collection_endpoints.o
MAIN_HEADER_FILES = ../endpoints/endpoints.h collection_endpoints.o
OBJECT_FILES = $(ENDPNTS) $(COLLECTION_OBJECTS)

BIN=bin
CC=gcc
TARGET=collections

all: main.o $(OBJECT_FILES)
	gcc -o $(TARGET) main.o $(OBJECT_FILES) -lsqlite3 -luuid

log.o: ../logging/log.c ../logging/log.h
	gcc -c ../logging/log.c

collection_test.o: collection_test.c collection_test.h
	gcc -c collection_test.c

collection_endpoints.o: collection_endpoints.c collection_endpoints.h
	gcc -c collection_endpoints.c

endpoints.o: ../endpoints/endpoints.c ../endpoints/endpoints.h
	gcc -c ../endpoints/endpoints.c

main.o: main.c $(MAIN_HEADER_FILES)
	gcc -c main.c

clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~

