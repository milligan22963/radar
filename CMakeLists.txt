cmake_minimum_required (VERSION 3.5)

# Debug specifics
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Release specifics
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Architecture
set (ARCH amd64)

set (AFM ${PROJECT_SOURCE_DIR}/gateway/afm)

# These were created from the source and placed in this specific location
set (PAHO_MQTT_INCLUDE /usr/local/include)
set (PAHO_MQTT_LIB /usr/local/lib)

set (JSON_INCLUDE /usr/local/include/nlohmann)

# For poco - include such as "Poco/Util/Application.h" so we know where it comes from
set (POCO_INCLUDE /usr/local/include)
set (POCO_LIB /usr/local/lib)

set (SYSTEM ${PROJECT_SOURCE_DIR}/gateway/shared)
set (SYSTEM_INCLUDE ${SYSTEM}/include)

# Output for install
set (LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set (BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

add_subdirectory(gateway)

