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
pacman -S --noconfirm --needed base-devel linux-headers kmod git
printf "Manual package installation...\n"
buildexist=$(mkdir builds)
cd builds
if [ -z "$buildexist" ]; then
	printf "Downloading Jansson...\n"
	curl -L -O http://www.digip.org/jansson/releases/jansson-2.7.tar.gz
	tar -xvf jansson-2.7.tar.gz
fi
cd jansson-2.7
printf "Installing Jansson...\n"
./configure
make
make check
make install
cd ..