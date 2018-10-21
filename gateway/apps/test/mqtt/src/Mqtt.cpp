/*--------------------------------------
  Radar
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>
#include <unistd.h>

#include "Mqtt.h"
#include "container/ContainerFactory.h"
#include "data/DataPacketFactory.h"

namespace afm
{
    namespace application
    {
        Mqtt::Mqtt()
            : m_logger(Poco::Logger::get("Mqtt"))
        {

        }

        Mqtt::~Mqtt()
        {
            m_pContainer = nullptr;
        }

        /**
         * Protected portions
         */
        void Mqtt::initialize(Application &self)
        {
            Application::initialize(self);

            m_pContainer = container::ContainerFactory::CreateContainer(container::ContainerType::MQTT_CONTAINER);

            if (m_pContainer != nullptr) {
                //
                container::ContainerOptions options;

                options[container::sc_target] = "ec2-18-191-138-192.us-east-2.compute.amazonaws.com";
                options["clientId"]  = "mqtttest";
                options["topic"] = {"test", "test2"};

                if (m_pContainer->Initialize(options) == true) {
                    m_logger.information("Application Initialized");
                } else {
                    m_logger.error("Unable to initialize container");
                }
            }
        }

        int Mqtt::main(const std::vector<std::string> &args)
        {
            data::IDataPacketSPtr pPacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::DATA);

            while (m_pContainer->ReadPacket(pPacket) == true) {
                m_logger.information("Read: %s", pPacket->ToJSON());
            }

            // wait for some time
            sleep(25);

            return 0;
        }
    }
}