# QualiQueue File System (QQFS) Installer

echo ""
echo "---- Installing QQFS ----"
echo ""

#### FUSE - update & install

sudo pacman -S build-essential git libfuse-dev libcurl4-openssl-dev libxml2-dev mime-support automake libtool

sudo pacman -S install pkg-config libssl-dev

git clone https://github.com/s3fs-fuse/s3fs-fuse

cd s3fs-fuse/

sudo bash ./autogen.sh

sudo ./configure --prefix=/usr --with-openssl

sudo make

sudo make install

cd ../

#### End FUSE install
#### Install QQFS

sudo bash ./autogen.sh

sudo ./configure

sudo make

sudo cp -f src/qqfs /usr/bin

sudo chmod 777 /usr/bin/qqfs

echo ""
echo "---- QQFS Installed ----"
echo ""

