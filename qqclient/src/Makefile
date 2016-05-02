CC=gcc
#OPT=-O3
#OPT=-g
WARN=-Wall
ERR=-Werror
STD=-std=gnu99

CFLAGS=$(OPT) $(WARN) $(ERR) $(STD)
COPTS=-ljansson -lpthread
CLINKS=libcommon_logging.o libcommon_qqjson.o libcommon_client_comm.o threadcommon.o receiver.o parser.o main.o

all: clean main

main: $(CLINKS)
	$(CC) $(CFLAGS) $(CLINKS) -o qqclient $(COPTS)

main.o: main.c
	$(CC) $(STD) -c main.c

threadcommon.o: threadcommon/common.c
	$(CC) $(STD) -c threadcommon/common.c -o threadcommon.o

receiver.o: receiver/receiver.c
	$(CC) $(STD) -c receiver/receiver.c

parser.o: parser/parser.c
	$(CC) $(STD) -c parser/parser.c

libcommon_qqjson.o: ../../libcommon/qqjson/qqjson.c
	$(CC) $(STD) -c ../../libcommon/qqjson/qqjson.c -o libcommon_qqjson.o

libcommon_logging.o: ../../libcommon/logging/logging.c
	$(CC) $(STD) -c ../../libcommon/logging/logging.c -o libcommon_logging.o

libcommon_client_comm.o: ../../libcommon/client/communication.c
	$(CC) $(STD) -c ../../libcommon/client/communication.c -o libcommon_client_comm.o

clean:
	rm -rf *.o qqclient