#!/bin/bash
# Assumes RHEL or derivatives
printf "Installing test environment components...\n"
vagranturl=https://releases.hashicorp.com/vagrant/1.8.1/vagrant_1.8.1_x86_64.rpm
repourl=http://download.virtualbox.org/virtualbox/rpm/el/virtualbox.repo
printf "Updating system...\n"
yum update
yum install binutils qt gcc make patch libgomp glibc-headers glibc-devel kernel-headers kernel-devel dkms
printf "Installing VirtualBox...\n"
curl -s $repourl -o virtualbox.repo
cp virtualbox.repo /etc/yum.repos.d/virtualbox.repo
yum install VirtualBox-5.0
service vboxdrv setup
printf "Installing Vagrant...\n"
curl -s $vagranturl -o vagrant.rpm
rpm --install vagrant.rpm
printf "Finished test environment component installation.\n"
printf "Please follow the supplied test environment installation instructions.\n"