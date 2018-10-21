/*--------------------------------------
  ContainerFactory
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <Poco/Logger.h>

#include "ContainerFactory.h"
#include "BluetoothContainer.h"
#include "FileContainer.h"
#include "MQTTContainer.h"
#include "WebContainer.h"
#include "SSLWebContainer.h"
#include "SQLiteContainer.h"

namespace afm
{
    namespace container
    {
        static const std::string sc_file = "file";
        static const std::string sc_http = "http";
        static const std::string sc_https = "https";
        static const std::string sc_mqtt = "mqtt";
        static const std::string sc_bluetooth = "bluetooth";
        static const std::string sc_sqlite = "sqlite";

        IContainerSPtr ContainerFactory::CreateContainer(ContainerType type)
        {
            IContainerSPtr pContainer = nullptr;

            switch (type) {
                case ContainerType::FILE_CONTAINER: {
                    pContainer = std::make_shared<FileContainer>();
                }
                break;
                case ContainerType::HTTP_CONTAINER: {
                    pContainer = std::make_shared<WebContainer>();
                }
                break;
                case ContainerType::HTTPS_CONTAINER: {
                    pContainer = std::make_shared<SSLWebContainer>();
                }
                break;
                case ContainerType::MQTT_CONTAINER: {
                    pContainer = std::make_shared<MQTTContainer>();
                }
                break;
                case ContainerType::BLUETOOTH_CONTAINER: {
                    pContainer = std::make_shared<BluetoothContainer>();
                }
                break;
                case ContainerType::SQLITE_DATABASE_CONTAINER: {
                    pContainer = std::make_shared<SQLiteContainer>();
                }
                break;
                default: {
                    Poco::Logger::get("ContainerFactory").error("Unable to create container");
                }
                break;
            }
            return pContainer;
        }

        ContainerType ContainerFactory::GetContainerType(const std::string &containerName)
        {
            ContainerType type = ContainerType::EMPTY_CONTAINER;

            if (containerName == sc_file) {
                type = ContainerType::FILE_CONTAINER;
            } else if (containerName == sc_http) {
                type = ContainerType::HTTP_CONTAINER;
            } else if (containerName == sc_https) {
                type = ContainerType::HTTPS_CONTAINER;
            } else if (containerName == sc_mqtt) {
                type = ContainerType::MQTT_CONTAINER;
            } else if (containerName == sc_bluetooth) {
                type = ContainerType::BLUETOOTH_CONTAINER;
            } else if (containerName == sc_sqlite) {
                type = ContainerType::SQLITE_DATABASE_CONTAINER;
            }

            return type;
        }

        // Internal/private
        ContainerFactory::ContainerFactory()
        {

        }

        ContainerFactory::~ContainerFactory()
        {

        }
    }
}
