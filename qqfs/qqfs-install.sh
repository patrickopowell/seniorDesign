# QualiQueue File System (QQFS) Installer

echo ""
echo "---- Installing QQFS ----"
echo ""

#### Install QQFS

sudo rm -f /usr/bin/qqfs

sudo rm -f /usr/local/bin/qqfs

sudo bash ./autogen.sh

sudo ./configure

sudo make

sudo cp -f src/qqfs /usr/bin

sudo chmod 777 /usr/bin/qqfs

echo ""
echo "---- QQFS Installed ----"
echo ""

