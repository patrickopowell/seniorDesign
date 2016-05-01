cp ../qqfs/src/qos_throttle.c ./qqfs

cp ../qqfs/src/qos_throttle.h ./qqfs

cp ../libcommon/client/communication.h ./qqfs

cp ../libcommon/client/communication.c ./qqfs

cp ../libcommon/logging/logging.h ./libcommon/logging

cp ../libcommon/logging/logging.c ./libcommon/logging

cd ./qqfs

gcc -Wall -c qos_throttle.c communication.c ../libcommon/logging/logging.c -O0 -fprofile-arcs -ftest-coverage -g

gcc -Wall -L/usr/local/lib -o qos_throttle_test qos_throttle_test.c qos_throttle.o communication.o logging.o -lcunit -pthread -lgcov