CC=gcc
WARN=-Wall
ERR=-Werror

CFLAGS=$(CC) $(WARN) $(ERR)

QOS_THROTTLE_OBJ=qos_throttle.o
QOS_MAIN_OBJ=main.o

all: qos_throttle main

qos_throttle: $(QOS_THROTTLE_OBJ)
	cd qos_throttle
	$(CC) -o qos_throttle $(CFLAGS) $(QOS_THROTTLE_OBJ)
	@echo "---------------------------------------------------------------"
	@echo "-----------    Storage QoS Throttle Compiled!     -----------"
	@echo "---------------------------------------------------------------"
	cd ..

main: $(QOS_MAIN_OBJ)
	cd main
	$(CC) -o main $(CFLAGS) $(QOS_MAIN_OBJ) -ljansson
	cd ..