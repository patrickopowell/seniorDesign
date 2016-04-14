# FUSE - update & install

sudo apt-get install build-essential git libfuse-dev libcurl4-openssl-dev libxml2-dev mime-support automake libtool

sudo apt-get install pkg-config libssl-dev

git clone https://github.com/s3fs-fuse/s3fs-fuse

cd s3fs-fuse/

sudo sh ./autogen.sh

./configure --prefix=/usr --with-openssl

make

sudo make install

cd ../

# End FUSE install
# Install BBFS

cd ../lib/bbfs

pwd

sudo sh ./autogen.sh

sudo ./configure CC=c99 CFLAGS=-g LIBS=-lposix

make

cd ../../qos_throttle

make

