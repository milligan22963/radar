#!/bin/bash

if [ ! -d release ]; then
  ./cfg_release.sh
fi

cd release

cores=`grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4}'`
make -j${cores} install

