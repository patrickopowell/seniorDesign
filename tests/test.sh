cp ../qqfs/src/qos_throttle.c .

cp ../qqfs/src/qos_throttle.h .

cp ../qqfs/src/shared_common.h .

gcc -Wall -c qos_throttle.c

gcc -Wall -L/usr/local/lib -o qos_throttle_test qos_throttle_test.c qos_throttle.o -lcunit -pthread