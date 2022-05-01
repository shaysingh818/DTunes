CC=gcc
TARGET=dtunes


all: main.o db.o playlist.o song.o urls.o  endpoints.o 
	gcc -o $(TARGET) main.o db.o playlist.o song.o urls.o endpoints.o  -lsqlite3 -lpthread -luuid

playlist.o: playlist.c playlist.h
	gcc -c playlist.c

song.o: song.c song.h
	gcc -c song.c

urls.o: urls.c urls.h
	gcc -c urls.c

db.o: db.c db.h
	gcc -c db.c

endpoints.o: endpoints.c endpoints.h
	gcc -c endpoints.c 

main.o: main.c db.h playlist.h song.h urls.h endpoints.h
	gcc -c main.c -lsqlite3

clean:
	rm $(TARGET)1
