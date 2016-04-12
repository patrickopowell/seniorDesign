CC=gcc
#OPT = -O3
#OPT = -g
WARN = -Wall
ERR = -Werror

CFLAGS = $(OPT) $(WARN) $(ERR)

all: clean main

main: main.o receiver.o parser.o qos-json.o
	$(CC) $(CFLAGS) main.o receiver.o parser.o qos-json.o -o main -ljansson

main.o: main.c
	$(CC) -c main.c

receiver.o: receiver.c
	$(CC) -c receiver/receiver.c

parser.o: parser.c
	$(CC) -c parser/parser.c

qos-json.o: qos-json.c
	$(CC) -c ../lib/qos-json/qos-json.c

clean:
	rm -rf *.o main