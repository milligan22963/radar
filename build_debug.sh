#!/bin/bash

if [ ! -d debug ]; then
  ./cfg_debug.sh
fi

cd debug

cores=`grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4}'`
make -j${cores} install
