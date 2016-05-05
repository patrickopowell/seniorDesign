CC=gcc
WARN=-Wall
ERR=-Werror
STD=-std=gnu99
DBG=-g

CFLAGS=$(OPT) $(WARN) $(ERR) $(STD) $(DBG)
COPTS=-ljansson -lpthread
CLINKS=libcommon_logging.o libcommon_qqjson.o libcommon_client_comm.o threadcommon.o receiver.o parser.o main.o

OFILE=../bin/qqclient

all: clean main

main: $(CLINKS)
	$(CC) $(CFLAGS) $(CLINKS) -o $(OFILE) $(COPTS)

main.o: main.c
	$(CC) $(STD) $(DBG) -c main.c

threadcommon.o: threadcommon/common.c
	$(CC) $(STD) $(DBG) -c threadcommon/common.c -o threadcommon.o

receiver.o: receiver/receiver.c
	$(CC) $(STD) $(DBG) -c receiver/receiver.c

parser.o: parser/parser.c
	$(CC) $(STD) $(DBG) -c parser/parser.c

libcommon_qqjson.o: ../../libcommon/qqjson/qqjson.c
	$(CC) $(STD) $(DBG) -c ../../libcommon/qqjson/qqjson.c -o libcommon_qqjson.o

libcommon_logging.o: ../../libcommon/logging/logging.c
	$(CC) $(STD) $(DBG) -c ../../libcommon/logging/logging.c -o libcommon_logging.o

libcommon_client_comm.o: ../../libcommon/client/communication.c
	$(CC) $(STD) $(DBG) -c ../../libcommon/client/communication.c -o libcommon_client_comm.o

clean:
	rm -rf *.o $(OFILE)