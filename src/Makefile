CC=gcc
TARGET=dtunes

all:
	$(CC) main.c playlist.c song.c endpoints.c db.c unit_tests.c  -lsqlite3 -o $(TARGET)

clean:
	rm $(TARGET)
