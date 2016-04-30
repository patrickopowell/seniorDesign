#### FUSE - update & install

#sudo pacman -S build-essential git libfuse-dev libcurl4-openssl-dev libxml2-dev mime-support automake libtool

#sudo pacman -S install pkg-config libssl-dev

#git clone https://github.com/s3fs-fuse/s3fs-fuse

#cd s3fs-fuse/

#sudo sh ./autogen.sh

#sudo ./configure --prefix=/usr --with-openssl

#sudo make

#sudo make install

#cd ../

#### End FUSE install
#### Install CUnit

#cd ../../client/src/lib/CUnit-2.1-2
 
#sudo ./configure --prefix=/usr
 
#sudo make
 
#sudo make install

#cd ../../../../qqfs/src
 
#### End CUnit install

cd ../
#### Install QQFS

sudo sh autogen.sh

sudo ./configure

sudo make

