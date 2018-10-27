# radar
Reception and Delivery of Ambient Readings

Relies on a number of projects:
POCO - https://github.com/pocoproject/poco
Paho - https://github.com/eclipse/paho.mqtt.c
JSON - https://github.com/nlohmann/json

Built using CMAKE.  

See dependencies.sh for all required build artifacts.  This will setup your system for building this project.  Run it as:
sudo dependencies.sh

Once done you should be ready to:
./cfg_debug.sh
./build_debug.sh

or:
./cfg_release.sh
./build_release.sh


Omce built run it as such:
sudo ./debug/staging/bin/Radar -cfg gateway/apps/Radar/scripts/Radar.cfg

You will want to modify the configuration file to point to your own mqtt broker if using an MQTT data source/sink.  The database portion is work in progress.  It is important to run as sudo in order to access the bluetooth hardware.  I haven't yet configured my account to run it directly without sudo.  The intent is that the application would be part of a service running on boot.
