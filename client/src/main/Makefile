CC=gcc
#OPT = -O3
#OPT = -g
WARN = -Wall
#ERR = -Werror

CFLAGS = $(OPT) $(WARN) $(ERR) $(INC) $(LIB)

SRC = qos_throttle.c
OBJ = qos_throttle.o

all: clean main
	@echo "\nCompilation Complete!"

main: main.o receiver.o parser.o qos-json.o
	$(CC) $(CFLAGS) main.o receiver.o parser.o qos-json.o -o main -ljansson
	@echo "---------------------------------------------------------------"
	@echo "-----------    Storage QoS Client Compiled!     -----------"
	@echo "---------------------------------------------------------------"

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