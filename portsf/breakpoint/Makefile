CC=gcc
TARGET=breakpoint

all: breakpoint.o 
	gcc -o $(TARGET) breakpoint.o

breakpoint.o: breakpoint.c breakpoint.h
	gcc -c breakpoint.c
