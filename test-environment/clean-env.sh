#!/bin/bash
# Shutdown and destroy existing VMs in environment.
# @author Remington Campbell <racampbe@ncsu.edu
printf 'Destroying existing Boxes...\n'
vagrant destroy --force
printf 'Boxes destroyed!\n'