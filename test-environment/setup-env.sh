#!/bin/bash
printf 'Configuring Vagrant environment...\n';
vagrant_dir='/qos/vagrant_env/';
vagrant_image_dir='/qos/vagrant_images/';
vm_dir='/qos/vm_images';
VAGRANT_HOME=$vagrant_image_dir;
export VAGRANT_HOME;
VAGRANT_CWD=$vagrant_dir;
export VAGRANT_CWD;
printf 'Generating Vagrantfile...\n';
./gen-vagrantfile.py;
if [ $? -gt 0 ]
then
	printf 'Issues encountered when generating Vagrantfile!\n';
	exit 1;
fi
printf 'Bringing up Vagrant Boxes...\n';
VBoxManage setproperty machinefolder $vm_dir;
vagrant up;
VBoxManage setproperty machinefolder default;
printf 'Finished bringing up test environment.\n';