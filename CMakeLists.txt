cmake_minimum_required (VERSION 3.5)

# Debug specifics
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Release specifics
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Architecture
set (ARCH amd64)

set (PAHO_MQTT_VERSION 1.2.1)
set (JSON_VERSION 3.1.2)
set (POCO_VERSION 1.9.0)

set (COTS ${PROJECT_SOURCE_DIR}/cots)
set (AFM ${PROJECT_SOURCE_DIR}/gateway/afm)

# These were created from the source and placed in this specific location
set (PAHO_MQTT_INCLUDE ${COTS}/paho/${PAHO_MQTT_VERSION}/include)
set (PAHO_MQTT_LIB ${COTS}/paho/${PAHO_MQTT_VERSION}/lib/${ARCH})

set (JSON_INCLUDE ${COTS}/json/${JSON_VERSION}/include)

# For poco - include such as "Poco/Util/Application.h" so we know where it comes from
set (POCO_INCLUDE ${COTS}/poco/poco-${POCO_VERSION}-all/afm-build/include)
set (POCO_LIB ${COTS}/poco/poco-${POCO_VERSION}-all/afm-build/lib/${ARCH})

set (SYSTEM ${PROJECT_SOURCE_DIR}/gateway/shared)
set (SYSTEM_INCLUDE ${SYSTEM}/include)

# Output for install
set (LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set (BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

add_subdirectory(gateway)

