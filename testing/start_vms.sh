#!/bin/bash
vm_name="VM Name"
qemu-system-i386 -name "$vm_name" -net nic,macaddr=$(qemu-mac-hasher.py "$vm_name") -net vde disk_image