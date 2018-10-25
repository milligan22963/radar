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

