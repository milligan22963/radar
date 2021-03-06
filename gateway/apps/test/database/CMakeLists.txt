set(SRC_FILES
    src/main.cpp
    src/Database.cpp
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

add_executable (DatabaseTest ${SRC_FILES})

target_link_libraries(
    DatabaseTest
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
install (TARGETS DatabaseTest DESTINATION ${BIN_DIR})
