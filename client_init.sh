#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib
export VSOMEIP_APPLICATION_NAME="IVI-Client"
export COMMONAPI_DEFAULT_BINDING="someip"
export VSOMEIP_CONFIGURATION="$PWD/client-config.json"
sudo ip route add 224.224.224.245 dev lo 2>/dev/null

echo "Environment was set for IVI Client"
echo "LD_LIBRARY_PATH was set"
echo "VSOMEIP_CONFIGURATION=$VSOMEIP_CONFIGURATION"
