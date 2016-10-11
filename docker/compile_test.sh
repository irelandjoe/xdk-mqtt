#!/bin/bash

cd $XDK_MQTT
make clean
make

if [ -d "/result" ]; then
  cp ./debug/relayrxdk.hex /result/
fi