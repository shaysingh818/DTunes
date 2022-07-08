# MAKEFILE FOR ISOLATED PHAEDRA COMPILATION

MAIN_HEADER_FILES = ../db/db.h ../endpoints/endpoints.h phaedra.h phaedra_endpoints.h
DB = db.o
ENDPNTS = endpoints.o
PHAEDRA = phaedra.o phaedra_endpoints.o
OBJECT_FILES =  $(DB) $(ENDPNTS) $(PHAEDRA) 

BIN=bin
CC=gcc
TARGET=phaedra


all: main.o db.o endpoints.o phaedra.o phaedra_endpoints.o
	gcc -o $(TARGET) main.o $(OBJECT_FILES) -lportaudio -lportsf -lm -lpthread -lsqlite3

phaedra.o: phaedra.c phaedra.h
	gcc -c phaedra.c

phaedra_endpoints.o: phaedra_endpoints.c phaedra_endpoints.h
	gcc -c phaedra_endpoints.c

endpoints.o: ../endpoints/endpoints.c ../endpoints/endpoints.h
	gcc -c ../endpoints/endpoints.c

db.o: ../db/db.c ../db/db.h
	gcc -c ../db/db.c 

main.o: main.c $(MAIN_HEADER_FILES)
	gcc -c main.c
