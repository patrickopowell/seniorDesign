CC=gcc
#OPT=-O3
#OPT=-g
WARN=-Wall
ERR=-Werror
STD=-std=gnu99

CFLAGS=$(OPT) $(WARN) $(ERR) $(STD)
COPTS=-ljansson -lpthread
CLINKS=main.o logging.o qos-json.o receiver.o parser.o

all: clean main

main: main.o logging.o qos-json.o receiver.o parser.o
	$(CC) $(CFLAGS) $(CLINKS) -o qqclient $(COPTS)

main.o: main.c
	$(CC) $(STD) -c main.c

receiver.o: receiver/receiver.c
	$(CC) $(STD) -c receiver/receiver.c

parser.o: parser/parser.c
	$(CC) $(STD) -c parser/parser.c

qos-json.o: ../lib/qos-json/qos-json.c
	$(CC) $(STD) -c ../lib/qos-json/qos-json.c

logging.o: ../lib/logging/logging.c
	$(CC) $(STD) -c ../lib/logging/logging.c

clean:
	rm -rf *.o main