/*--------------------------------------
  Settings
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "container/ContainerFactory.h"
#include "Settings.h"

namespace afm 
{
    namespace common
    {
        Settings::Settings()
            : m_logger(Poco::Logger::get("Settings"))
        {

        }

        Settings::~Settings()
        {

        }

        bool Settings::Initialize(const std::string &configurationFile, container::IContainerNotificationSPtr pNotificationRecipient)
        {
            bool success = false;

            nlohmann::json configurationData;

            std::ifstream configurationStream(configurationFile);

            if (configurationStream.is_open() == true) {
                std::string containerType;
                std::string containerId;

                configurationStream >> configurationData;

                success = true;
                for (auto container : configurationData["containers"]) {
                    containerId = container["id"];
                    containerType = container["type"];

                    m_logger.information("Processing container: %s", containerId);

                    container::ContainerType type = container::ContainerFactory::GetContainerType(containerType);

                    if (type != container::ContainerType::EMPTY_CONTAINER) {
                        container::IContainerSPtr pContainer = container::ContainerFactory::CreateContainer(type);

                        if (pContainer != nullptr) {
                            if (pContainer->Initialize(container, pNotificationRecipient) == true) {
                                m_containers.insert(std::make_pair(containerId, pContainer));
                            } else {
                                success = false;
                                m_logger.error("Unable to initialize container: %s", containerId);
                            }
                        }
                    }
                }
            } else {
                m_logger.error("Unable to open configuration file: %s", configurationFile);
            }

            return success;
        }

        container::IContainerSPtr Settings::GetContainer(const std::string &containerId)
        {
            container::IContainerSPtr pContainer = nullptr;

            ContainerMap::iterator iter = m_containers.find(containerId);

            if (iter != m_containers.end()) {
                pContainer = iter->second;
            }

            return pContainer;
        }

        void Settings::Shutdown()
        {
            for (auto container : m_containers) {
                container.second->Shutdown();
            }
        }
    }
}
