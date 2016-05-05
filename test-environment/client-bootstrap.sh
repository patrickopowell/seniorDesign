#!/bin/bash
# @author Remington Campbell <rem.campbell@gmail.com>
# Assumes Arch Linux
printf 'Downloading ranked mirrors...\n'
if [ -f /etc/pacman.d/mirrorlist ]; then
  cp /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist_$(date +%Y%m%d%H%M)
else
  printf 'Warning: no /etc/pacman.d/mirrorlist for backup\n'
fi
curl -s 'https://www.archlinux.org/mirrorlist/?country=US&protocol=http&ip_version=4&use_mirror_status=on' -o mirrorlist
if [ $? -gt 0 ]; then
	printf 'Mirrorlist download failed, not replacing!\n'
	printf 'This is going to be slow.\n'
else
	printf 'Mirrorlist download successful!\n'
	sed -i 's/^#Server/Server/g' mirrorlist
	mv mirrorlist /etc/pacman.d/mirrorlist
fi
printf 'Force updating system...\n'
pacman -Syyu --noconfirm
printf 'Installing development packages...\n'
pacman -S --noconfirm --needed base-devel linux-headers kmod git fuse libfuse-dev automake libtool
printf 'Manual package installation...\n'
cd /vagrant/
mkdir autobuild
if [ $? -eq 0 ]; then
	printf 'Sources for installation not present!\n'
	printf 'QualiQueue and Jansson sources are required to be present in autobuild in shared folder.\n'
	exit 1
fi
cd autobuild
printf 'Installing Jansson...\n'
mkdir jansson-2.7
if [ $? -eq 0 ]; then
	rmdir jansson-2.7
	printf 'Unpacking Jansson for the first time...\n'
	tar -xvf jansson-2.7.tar.gz
fi
cd jansson-2.7
if [ $? -eq 0 ]; then
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
else
	printf 'Jansson folder not present!\n'
fi
printf 'Installing QualiQueue client components...\n'
cd qqbuild
if [ $? -eq 0 ]; then
	bash install-client.sh
	cd ..
else
	printf 'QualiQueue folder not present!\n'
fi