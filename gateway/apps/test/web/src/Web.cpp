/*--------------------------------------
  Radar
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>
#include "Web.h"
#include "data/DataPacketFactory.h"

namespace afm
{
    namespace application
    {
        Web::Web()
            : m_logger(Poco::Logger::get("Radar"))
        {

        }

        Web::~Web()
        {
            m_pContainer = nullptr;
        }

        /**
         * Protected portions
         */
        void Web::initialize(Application &self)
        {
            Application::initialize(self);

            m_pContainer = container::ContainerFactory::CreateContainer(container::ContainerType::HTTP_CONTAINER);

            if (m_pContainer != nullptr) {
                //
                container::ContainerOptions options;

                options[container::sc_target] =  "http://localhost/index.php";

                if (m_pContainer->Initialize(options) == true) {
                    m_logger.information("Application Initialized");
                } else {
                    m_logger.error("Unable to initialize container");
                }
            }
        }

        int Web::main(const std::vector<std::string> &args)
        {
            int count = 0;
            data::IDataPacketSPtr pPacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::DATA);

            pPacket->SetValue("count", "0");
            while (m_pContainer->ReadPacket(pPacket) == true) {
                m_logger.information("Read: %s", pPacket->ToJSON());

                count++;

                if (count > 5) {
                    break;
                }

                char buffer[16];
                sprintf(buffer, "%d", count);
                pPacket->SetValue("count", buffer);
            }

            count = 0;
            while (m_pContainer->WritePacket(pPacket) == true) {
                m_pContainer->ReadPacket(pPacket);
                m_logger.information("Write: %s", pPacket->ToJSON());

                count++;

                if (count > 5) {
                    break;
                }

                char buffer[16];
                sprintf(buffer, "%d", count);
                pPacket->SetValue("count", buffer);
            }

            return 0;
        }
    }
}