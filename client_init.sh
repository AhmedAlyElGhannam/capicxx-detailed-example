#!/bin/bash

sudo ip route add 224.224.224.245 dev wlp82s0 # replace with valid network interface
export VSOMEIP_CONFIGURATION="${PWD}/config.json"
