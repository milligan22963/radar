/*--------------------------------------
  Radar
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>
#include "File.h"
#include "data/DataPacketFactory.h"

namespace afm
{
    namespace application
    {
        File::File()
            : m_logger(Poco::Logger::get("File"))
        {

        }

        File::~File()
        {
            m_pContainer = nullptr;
        }

        /**
         * Protected portions
         */
        void File::initialize(Application &self)
        {
            Application::initialize(self);

            m_pContainer = container::ContainerFactory::CreateContainer(container::ContainerType::FILE_CONTAINER);

            if (m_pContainer != nullptr) {
                //
                container::ContainerOptions options;

                options[container::sc_target] = "/home/daniel/Development/test.dat";

                if (m_pContainer->Initialize(options) == true) {
                    m_logger.information("Application Initialized");
                } else {
                    m_logger.error("Unable to initialize container");
                }
            }
        }

        int File::main(const std::vector<std::string> &args)
        {
            data::IDataPacketSPtr pPacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::DATA);

            while (m_pContainer->ReadPacket(pPacket) == true) {
                m_logger.information("Read: %s", pPacket->ToJSON());
            }

            return 0;
        }
    }
}