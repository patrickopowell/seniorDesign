CC=gcc
#OPT = -O3
#OPT = -g
WARN = -Wall
#ERR = -Werror

CFLAGS = $(OPT) $(WARN) $(ERR) $(INC) $(LIB)

SRC = qos_throttle.c
OBJ = qos_throttle.o

all: clean qos_throttle
	@echo "\nCompilation Complete!"

qos_throttle: $(OBJ)
	$(CC) -o qos_throttle $(CFLAGS) $(OBJ)
	@echo "---------------------------------------------------------------"
	@echo "-----------    Storage QoS Throttle Compiled!     -----------"
	@echo "---------------------------------------------------------------"
	
clean:
	rm -rf *.o qos_throttle