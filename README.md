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


Once built run it as such:
sudo ./debug/staging/bin/Radar -cfg gateway/apps/Radar/scripts/Radar.cfg

You will want to modify the configuration file to point to your own mqtt broker if using an MQTT data source/sink.  The database portion is work in progress.  It is important to run as sudo in order to access the bluetooth hardware.  I haven't yet configured my account to run it directly without sudo.  The intent is that the application would be part of a service running on boot.


Basic data captured from a BT enabled temp/humidity sensor:
Read: {"address":"00a050072a1c","humidity":"78.4727","rawData":"043e2a020103001c2a0750a0001e0201041aff4c0002150005000100001000800000805f9b01310010ad5ec3a9","rssi":"-87","temperature":"290.672","timeStamp":"1541893330.236745539"}
Read: {"address":"00a050072a1c","humidity":"78.4727","rawData":"043e2a020103001c2a0750a0001e0201041aff4c0002150005000100001000800000805f9b01310010ad5ec3a1","rssi":"-95","temperature":"290.672","timeStamp":"1541893438.975970453"}
Read: {"address":"00a050072a1c","humidity":"77.0078","rawData":"043e2a020103001c2a0750a0001e0201041aff4c0002150005000100001000800000805f9b01310010aa5ec3ac","rssi":"-84","temperature":"290.672","timeStamp":"1541893547.105631145"}
Read: {"address":"00a050072a1c","humidity":"77.0078","rawData":"043e2a020103001c2a0750a0001e0201041aff4c0002150005000100001000800000805f9b01310010aa5ec3ac","rssi":"-84","temperature":"290.672","timeStamp":"1541893654.291748426"}
