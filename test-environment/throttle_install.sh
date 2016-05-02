sudo rm -rf 2016springTeam28/

git clone https://github.ncsu.edu/engr-csc-sdc/2016springTeam28.git

cd 2016springTeam28

git checkout development

chmod -R 777 .

cd qqfs/src

echo ""
echo "---- Installing QQFS ----"
echo ""

sh qos_throttle.sh

echo ""
echo "---- QQFS Installed ----"
echo ""

cd ../example/

sudo chmod 777 -R .

sudo cp ../src/qqfs /usr/bin

sudo chmod 777 /usr/bin/qqfs

echo ""
echo "---- Mounting QQFS ----"
echo ""

/usr/bin/qqfs rootdir mountdir

ls -lR

fusermount -u mountdir

ls -lR

cd ../../tests

echo ""
echo "---- Un-Mounted QQFS ----"
echo ""

cd ../../tests/

echo ""
echo "---- preparing tests ----"
echo ""

sh test.sh

echo ""
echo "---- tests prepared ----"
echo ""

cd qqfs

echo ""
echo "---- running tests ----"
echo ""

./qos_throttle_test

gcov qos_throttle.c ../libcommon/logging/logging.c communication.c -o .
