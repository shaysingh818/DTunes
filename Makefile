# MAKE FILE FOR DTUNES
# SUB MODULES ARE THE OBJECTS VARIABLES
# LOADER/DRIVER FILES are the main header files needed

PLAYLIST_OBJECTS= playlist.o playlist_endpoints.o playlist_test.o
SONG_OBJECTS= song.o song_test.o song_endpoints.o 
URLS_OBJECTS= urls.o urls_test.o urls_endpoints.o 
DB_OBJECTS = db.o db_test.o db_endpoints.o endpoints.o
MAIN_HEADER_FILES = endpoints/endpoints.h audiofile/song_endpoints.h playlist/playlist_endpoints.h urls/urls_endpoints.h db/db_endpoints.h
OBJECT_FILES = $(DB_OBJECTS) $(PLAYLIST_OBJECTS) $(SONG_OBJECTS) $(URLS_OBJECTS)

BIN=bin
CC=gcc
TARGET=dtunes

all: main.o $(OBJECT_FILES)
	gcc -o $(TARGET) main.o $(OBJECT_FILES) -lsqlite3 -lpthread -luuid

endpoints.o: endpoints/endpoints.c endpoints/endpoints.h
	gcc -c endpoints/endpoints.c

playlist.o: playlist/playlist.c playlist/playlist.h
	gcc -c playlist/playlist.c

playlist_endpoints.o: playlist/playlist_endpoints.c playlist/playlist_endpoints.h
	gcc -c playlist/playlist_endpoints.c

playlist_test.o: playlist/playlist_test.c playlist/playlist_test.h
	gcc -c playlist/playlist_test.c

song.o: audiofile/song.c audiofile/song.h
	gcc -c audiofile/song.c

song_test.o: audiofile/song_test.c audiofile/song_test.h
	gcc -c audiofile/song_test.c

song_endpoints.o: audiofile/song_endpoints.c audiofile/song_endpoints.h
	gcc -c audiofile/song_endpoints.c

urls.o: urls/urls.c urls/urls.h
	gcc -c urls/urls.c

urls_test.o: urls/urls_test.c urls/urls_test.h
	gcc -c urls/urls_test.c

urls_endpoints.o: urls/urls_endpoints.c urls/urls_endpoints.h
	gcc -c urls/urls_endpoints.c

db.o: db/db.c db/db.h
	gcc -c db/db.c 

db_test.o: db/db_test.c db/db_test.h
	gcc -c db/db_test.c

db_endpoints.o: db/db_endpoints.c db/db_endpoints.h
	gcc -c db/db_endpoints.c

main.o: main.c $(MAIN_HEADER_FILES)
	gcc -c main.c -lsqlite3

clean_binary:	
	rm $(TARGET)

clean:
	rm -f *.o *~

