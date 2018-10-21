/*--------------------------------------
  Database

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>
#include "Database.h"
#include "data/DataPacketFactory.h"

namespace afm
{
    namespace application
    {
        Database::Database()
            : m_logger(Poco::Logger::get("Database"))
        {

        }

        Database::~Database()
        {
            m_pContainer = nullptr;
        }

        /**
         * Protected portions
         */
        void Database::initialize(Application &self)
        {
            Application::initialize(self);

            m_logger.information("Initializing application");
            m_pContainer = container::ContainerFactory::CreateContainer(container::ContainerType::SQLITE_DATABASE_CONTAINER);

            if (m_pContainer != nullptr) {
                m_logger.information("Container built, moving on to initialize");
                container::ContainerOptions options;

                options[container::sc_target] = "/tmp/test.db";

                if (m_pContainer->Initialize(options) == true) {
                    m_logger.information("Application Initialized");
                } else {
                    m_logger.error("Unable to initialize container");
                }
            }
        }

        int Database::main(const std::vector<std::string> &args)
        {
            if (m_pContainer->IsReady() == true) {
                data::IDataPacketSPtr pPacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::DATA);

                while (m_pContainer->ReadPacket(pPacket) == true) {
                    m_logger.information("Read: %s", pPacket->ToJSON());
                }
            } else {
                m_logger.error("Container isn't ready.");
            }
            return 0;
        }
    }
}