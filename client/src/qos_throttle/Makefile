CC=gcc
#OPT = -O3
#OPT = -g
WARN = -Wall
ERR = -Werror

CFLAGS = $(OPT) $(WARN) $(ERR) $(INC) $(LIB)

SIM_SRC = qos_throttle.c
SIM_OBJ = qos_throttle.o

all: qos_throttle

qos_throttle: $(SIM_OBJ)
	$(CC) -o qos_throttle $(CFLAGS) $(SIM_OBJ)
	@echo "---------------------------------------------------------------"
	@echo "-----------    Storage QoS Throttle Compiled!     -----------"
	@echo "---------------------------------------------------------------"