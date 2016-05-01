cp ../qqfs/src/qos_throttle.c ./qqfs

cp ../qqfs/src/qos_throttle.h ./qqfs

cp ../client/src/main/shared_common.h .

cp ../client/src/main/shared_common.c .

cd ./qqfs

gcc -Wall -c qos_throttle.c shared_common.c -O0 -fprofile-arcs -ftest-coverage -g

gcc -Wall -L/usr/local/lib -o qos_throttle_test qos_throttle_test.c qos_throttle.o shared_common.o -lcunit -pthread -lgcov