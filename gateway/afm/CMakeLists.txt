
set(APPLICATION_SRC_FILES
    application/src/AFMApp.cpp
)

set(COMMON_SRC_FILES
    common/src/BluetoothHCI.cpp
    common/src/GenericSystem.cpp
    common/src/Settings.cpp
    common/src/SystemFactory.cpp
)

set(CONTAINER_SRC_FILES
    container/src/Container.cpp
    container/src/DatabaseContainer.cpp
    container/src/BluetoothContainer.cpp
    container/src/FileContainer.cpp
    container/src/MQTTContainer.cpp
    container/src/WebContainer.cpp
    container/src/SSLWebContainer.cpp
    container/src/SQLiteContainer.cpp
    container/src/ContainerFactory.cpp
)

set(DATA_SRC_FILES
    data/src/BluetoothData.cpp
    data/src/DataPacket.cpp
    data/src/DataPacketFactory.cpp
)

set(DATABASE_SRC_FILES
    database/src/Column.cpp
    database/src/Table.cpp
    database/src/TableFactory.cpp
)

include_directories(
    . 
    ..
    application
    application/internal
    common
    common/internal
    container
    container/internal
    data
    data/internal
    database
    database/internal
    ${PAHO_MQTT_INCLUDE}
    ${JSON_INCLUDE}
    ${POCO_INCLUDE}
    ${SYSTEM_INCLUDE}
)

add_library (afm 
    ${APPLICATION_SRC_FILES}
    ${COMMON_SRC_FILES}
    ${CONTAINER_SRC_FILES}
    ${DATA_SRC_FILES}
    ${DATABASE_SRC_FILES}
    )

install (TARGETS afm DESTINATION ${LIB_DIR})
