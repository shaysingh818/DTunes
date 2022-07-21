ENDPNTS = endpoints.o
PHAEDRA = collection.o collection_endpoints.o
MAIN_HEADER_FILES = ../endpoints/endpoints.h collection.h collection_endpoints.h
OBJECT_FILES =  $(DB) $(ENDPNTS) $(PHAEDRA)

BIN=bin
CC=gcc
TARGET=collections

all: main.o endpoints.o collection.o collection_endpoints.o
	gcc -o $(TARGET) main.o $(OBJECT_FILES) -lsqlite3 -luuid

collection.o: collection.c collection.h
	gcc -c collection.c

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

