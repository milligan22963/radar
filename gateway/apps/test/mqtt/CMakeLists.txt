set(SRC_FILES
    src/main.cpp
    src/Mqtt.cpp
)

include_directories(
    .
    internal
    ${AFM}
    ${POCO_INCLUDE}
    ${JSON_INCLUDE}
)

link_directories(
    ${POCO_LIB}
    ${PAHO_MQTT_LIB}
    ${LIB_DIR}
)

add_executable (MqttTest ${SRC_FILES})

target_link_libraries(
    MqttTest
    afm
    PocoFoundation
    PocoUtil
    PocoNet
    PocoNetSSL
    PocoData
    PocoDataSQLite
    paho-mqtt3as
    pthread
)
install (TARGETS MqttTest DESTINATION ${BIN_DIR})
