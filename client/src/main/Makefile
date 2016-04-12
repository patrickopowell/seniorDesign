CC=gcc
#OPT = -O3
#OPT = -g
WARN = -Wall
ERR = -Werror
STD = -std=c99

CFLAGS = $(OPT) $(WARN) $(ERR) $(STD)

all: clean main

main: main.o receiver.o parser.o qos-json.o
	$(CC) $(CFLAGS) main.o receiver.o parser.o qos-json.o -o main -ljansson

main.o: main.c
	$(CC) $(STD) -c main.c

receiver.o: receiver.c
	$(CC) $(STD) -c receiver/receiver.c

parser.o: parser.c
	$(CC) $(STD) -c parser/parser.c

qos-json.o: qos-json.c
	$(CC) $(STD) -c ../lib/qos-json/qos-json.c

clean:
	rm -rf *.o main