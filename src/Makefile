CC=gcc
TARGET=dtunes


all: main.o db.o playlist.o song.o endpoints.o 
	gcc -o $(TARGET) main.o db.o playlist.o song.o endpoints.o  -lsqlite3

playlist.o: playlist.c playlist.h
	gcc -c playlist.c

song.o: song.c song.h
	gcc -c song.c

db.o: db.c db.h
	gcc -c db.c

endpoints.o: endpoints.c endpoints.h
	gcc -c endpoints.c 

test.o: test.c 
	gcc -c test.c

main.o: main.c db.h playlist.h song.h endpoints.h
	gcc -c main.c -lsqlite3

clean:
	rm $(TARGET)1
