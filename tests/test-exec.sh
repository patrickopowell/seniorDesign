cp ../qqfs/src/qos_throttle.c ./qqfs

cp ../qqfs/src/qos_throttle.h ./qqfs

cp ../libcommon/client/communication.h ./qqfs

cp ../libcommon/client/communication.c ./qqfs

cp ../libcommon/logging/logging.h ./logging

cp ../libcommon/logging/logging.c ./logging

cp ../qqclient/src/main.c ./qqclient

cp ../qqclient/src/main.h ./qqclient

cp ../qqclient/src/parser/parser.c ./parser

cp ../qqclient/src/parser/parser.h ./parser

cp ../qqclient/src/receiver/receiver.c ./receiver

cp ../qqclient/src/receiver/receiver.h ./receiver

cp ../qqclient/src/threadcommon/common.c ./threadcommon/

cp ../qqclient/src/threadcommon/common.h ./threadcommon/

cd ./qqfs

gcc -Wall -c qos_throttle.c communication.c ../logging/logging.c -O0 -fprofile-arcs -ftest-coverage -g

gcc -Wall -L/usr/local/lib -o qos_throttle_test qos_throttle_test.c qos_throttle.o communication.o logging.o -lcunit -pthread -lgcov

./qos_throttle_test

gcov qos_throttle.c ../libcommon/logging/logging.c communication.c -o .

cd ../qqclient

gcc -Wall -L/usr/local/lib -o qqclient_test qqclient_test.c main.c ../parser/parser.c ../receiver/receiver.c ../threadcommon/common.c -O0 -fprofile-arcs -ftest-coverage -g

gcc -Wall -L/usr/local/lib -o qqclient_test qqclient_test.c main.o parser.o receiver.o common.o -lcunit -pthread -lgcov

./qqclient_test

gcov main.c ../parser/parser.c ../receiver/receiver.c ../threadcommon/common.c -o .

cd ../