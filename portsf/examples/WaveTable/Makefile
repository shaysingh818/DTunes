BIN=bin
CC=gcc
TARGET=wavetable

OBJECT_FILES = main.o waveform.o

all: $(OBJECT_FILES)
	gcc -o $(TARGET) $(OBJECT_FILES) -lportsf -lm

waveform.o: waveform.c waveform.h
	gcc -c waveform.c

main.o: main.c
	gcc -c main.c


clean_binary:   
	rm $(TARGET)

clean:
	rm -f *.o *~

