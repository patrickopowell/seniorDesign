sudo rm -rf 2016springTeam28/

git clone https://github.ncsu.edu/engr-csc-sdc/2016springTeam28.git

cd 2016springTeam28

git checkout development

chmod -R 777 .

cd qqfs/src

sh qos_throttle.sh

cd ../example/

sudo chmod 777 -R .

sudo chmod 777 -R ../src

../src/qqfs rootdir mountdir

ls -lR

fusermount -u mountdir

ls -lR

cd ~/QualiQueue/2016springTeam28/tests

sh test.sh

./qos_throttle_test
