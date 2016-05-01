CC=gcc
#OPT=-O3
#OPT=-g
WARN=-Wall
ERR=-Werror
STD=-std=gnu99

CFLAGS=$(OPT) $(WARN) $(ERR) $(STD)
COPTS=-ljansson -lpthread
CLINKS=common.o shared_common.o main.o logging.o qq-json.o receiver.o parser.o

all: clean main

main: common.o shared_common.o main.o logging.o qq-json.o receiver.o parser.o
	$(CC) $(CFLAGS) $(CLINKS) -o qqclient $(COPTS)

main.o: main.c
	$(CC) $(STD) -c main.c

receiver.o: receiver/receiver.c
	$(CC) $(STD) -c receiver/receiver.c

parser.o: parser/parser.c
	$(CC) $(STD) -c parser/parser.c

qq-json.o: ../lib/qq-json/qq-json.c
	$(CC) $(STD) -c ../lib/qq-json/qq-json.c

logging.o: ../lib/logging/logging.c
	$(CC) $(STD) -c ../lib/logging/logging.c

common.o: common.c
	$(CC) $(STD) -c common.c

shared_common.o: shared_common.c
	$(CC) $(STD) -c shared_common.c

clean:
	rm -rf *.o main