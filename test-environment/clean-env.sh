#!/bin/bash
printf 'Destroying existing Boxes...\n';
vagrant destroy --force;
printf 'Boxes destroyed!\n';