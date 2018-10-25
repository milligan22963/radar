#!/bin/bash

BASE_DIR=`pwd`
PAHO_BUILD_DIR=build
JSON_BUILD_DIR=build

apt-get install libssl-dev \
	libpoco-dev \
	build-essential \
	gcc \
	make \
	cmake \
	cmake-gui \
	cmake-curses-gui

git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
if [ -d $PAHO_BUILD_DIR ]; then
    rm -rf $PAHO_BUILD_DIR
fi
mkdir -p $PAHO_BUILD_DIR
cd $PAHO_BUILD_DIR
mkdir -p /usr/local/include/paho
cmake -DCMAKE_INSTALL_INCLUDEDIR=include/paho -DPAHO_WITH_SSL=TRUE ../
make
make install

cd $BASE_DIR

git clone https://github.com/nlohmann/json.git
cd json
if [ -d $JSON_BUILD_DIR ]; then
    rm -rf $JSON_BUILD_DIR
fi
mkdir -p $JSON_BUILD_DIR
cd $JSON_BUILD_DIR
cmake ../
make
make install

cd $BASE_DIR
