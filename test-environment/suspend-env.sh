#!/bin/bash
# Suspend (sleep) existing VMs in environment.
# Requires more disc space.
# @author Remington Campbell <racampbe@ncsu.edu
printf 'Suspending test environment...\n'
vagrant suspend
printf 'Environment suspended!\n'