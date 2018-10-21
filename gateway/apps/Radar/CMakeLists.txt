set(SRC_FILES
    src/main.cpp
    src/Radar.cpp
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

add_executable (Radar ${SRC_FILES})

target_link_libraries(
    Radar
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
install (TARGETS Radar DESTINATION ${BIN_DIR})
