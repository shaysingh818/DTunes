CC=gcc
TARGET=dtunes

all: main.o db.o playlist.o song.o
	gcc -o $(TARGET) main.o db.o playlist.o song.o -lsqlite3 -luuid 

playlist.o: playlist.c playlist.h
	gcc -c playlist.c

song.o: song.c song.h
	gcc -c song.c

db.o: db.c db.h
	gcc -c db.c

main.o: main.c db.h playlist.h song.h
	gcc -c main.c -lsqlite3

clean:
	rm $(TARGET)1
