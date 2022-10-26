BIN=bin
CC=gcc
TARGET=keyboard_input

FLAGS = -lncurses

all: main.o $(OBJECT_FILES)
	gcc -o $(TARGET) main.o $(OBJECT_FILES) $(FLAGS)

main.o: main.c 
	gcc -c main.c

clean_binary:	
	rm $(TARGET)

clean:
	rm -f *.o *~
