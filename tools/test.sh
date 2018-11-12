#!/bin/bash

export MQTT_C_CLIENT_TRACE=ON
export MQTT_C_CLIENT_TRACE_LEVEL=MAXIMUM

cd ..
./debug/staging/bin/Radar -cfg gateway/apps/Radar/scripts/Radar.cfg 
cd -
