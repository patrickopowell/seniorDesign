#!/bin/bash

printf "Downloading ranked mirrors...\n"
if [ -f /etc/pacman.d/mirrorlist ]; then
  cp /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist_$(date +%Y%m%d%H%M)
else
  printf "Warning: no /etc/pacman.d/mirrorlist for backup\n"
fi
curl -s "https://www.archlinux.org/mirrorlist/?country=US&protocol=http&ip_version=4&use_mirror_status=on" -o mirrorlist
if [ $? -gt 0 ]; then
	printf "Mirrorlist download failed, not replacing!\n"
else
	printf "Mirrorlist download successful!\n"
	sed -i 's/^#Server/Server/g' mirrorlist
	mv mirrorlist /etc/pacman.d/mirrorlist
fi
printf "Force updating system...\n"
pacman -Syyu --noconfirm
printf "Installing packages...\n"
pacman -S --noconfirm --needed base-devel linux-headers kmod git fuse
printf "Manual package installation...\n"
cd /vagrant/
printf "Adding QualiQueue SSH key...\n"
cp qq.key /home/vagrant/.ssh/qq.key
cp ssh.config /home/vagrant/.ssh/config
chown vagrant:vagrant /home/vagrant/.ssh/qq.key
chown vagrant:vagrant /home/vagrant/.ssh/config
chmod 600 /home/vagrant/.ssh/qq.key
chmod 700 /home/vagrant/.ssh/config
buildexist=$(mkdir builds)
cd builds
if [ -z "$buildexist" ]; then
	printf "Downloading Jansson...\n"
	curl -L -O http://www.digip.org/jansson/releases/jansson-2.7.tar.gz
	tar -xvf jansson-2.7.tar.gz
	printf "Downloading QualiQueue...\n"
	runuser -l vagrant -c 'cd /vagrant/builds && git clone git@github.ncsu.edu:engr-csc-sdc/2016springTeam28.git'
	cd 2016springTeam28
	git checkout development
fi
cd jansson-2.7
printf "Installing Jansson...\n"
./configure
make
make check
make install
# if Arch
currpath=$(pwd)
cd /usr/local/lib
ln -s /usr/local/lib/libjansson.so.4 /usr/lib/libjansson.so.4
ldconfig
cd $currpath
# endif
cd ..
cd 2016springTeam28
bash installall.sh
cd ..