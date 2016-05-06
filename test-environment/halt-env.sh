#!/bin/bash
# Shutdown existing VMs in environment.
# @author Remington Campbell <racampbe@ncsu.edu
printf 'Halting test environment...\n'
vagrant halt
printf 'Environment halted!\n'