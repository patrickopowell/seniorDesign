cp ../qqfs/src/qos_throttle.c ./qqfs

cp ../qqfs/src/qos_throttle.h ./qqfs

cp ../client/src/main/shared_common.h ./qqfs

cp ../client/src/main/shared_common.c ./qqfs

cp ../client/src/lib/logging/logging.h ./lib/logging

cp ../client/src/lib/logging/logging.c ./lib/logging

cd ./qqfs

gcc -Wall -c qos_throttle.c shared_common.c ../lib/logging/logging.c -O0 -fprofile-arcs -ftest-coverage -g

gcc -Wall -L/usr/local/lib -o qos_throttle_test qos_throttle_test.c qos_throttle.o shared_common.o ../lib/logging/logging.o -lcunit -pthread -lgcov