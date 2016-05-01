cp ../qqfs/src/qos_throttle.c .

cp ../qqfs/src/qos_throttle.h .

cp ../client/src/lib/shared_common.h .

cp ../client/src/lib/shared_common.c .

gcc -Wall -c qos_throttle.c -O0 -fprofile-arcs -ftest-coverage -g

gcc -Wall -L/usr/local/lib -o qos_throttle_test qos_throttle_test.c qos_throttle.o -lcunit -pthread -lgcov