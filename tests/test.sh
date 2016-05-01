mkdir src

cp ../qqfs/src/qos_throttle.c ./src

cp ../qqfs/src/qos_throttle.h ./src

cd ./src

#cp ../client/src/main/shared_common.h .

#cp ../client/src/main/shared_common.c .

gcc -Wall -c qos_throttle.c -O0 -fprofile-arcs -ftest-coverage -g

gcc -Wall -L/usr/local/lib -o qos_throttle_test qos_throttle_test.c qos_throttle.o -lcunit -pthread -lgcov