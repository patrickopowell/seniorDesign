#!/bin/bash
vagrant_dir='/qos/vagrant_env/';
vagrant_image_dir='/qos/vagrant_images/';
vm_dir='/qos/vm_images';
VAGRANT_HOME=$vagrant_image_dir;
export VAGRANT_HOME;
VAGRANT_CWD=$vagrant_dir;
export VAGRANT_CWD;
printf 'Bringing up Vagrant Boxes...\n';
VBoxManage setproperty machinefolder $vm_dir;
vagrant up;
VBoxManage setproperty machinefolder default;
printf 'Finished bringing up test environment.\n';