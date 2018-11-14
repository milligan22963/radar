/*--------------------------------------
  DatabaseContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <Poco/Data/Session.h>

#include "database/TableFactory.h"
#include "DatabaseContainer.h"

namespace afm
{
    namespace container
    {
        static const std::string sc_hostName = "host";
        static const std::string sc_port = "port";
        static const std::string sc_userName = "username";
        static const std::string sc_password = "password";

        static const std::string sc_tables = "tables";

        DatabaseContainer::DatabaseContainer()
            : Container()
        {

        }

        DatabaseContainer::~DatabaseContainer()
        {
            m_tables.clear();

            m_pDBSession = nullptr;
        }

        bool DatabaseContainer::Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient)
        {
            bool success = Container::Initialize(options, pNotificationRecipient);

            if (success == true) {
                success = false; // assume bad until we have a connection

                if (options.find(sc_hostName) != options.end()) {
                    m_hostName = options[sc_hostName];
                }
                if (options.find(sc_port) != options.end()) {
                    m_port = options[sc_port].get<uint32_t>();
                }
                if (options.find(sc_userName) != options.end()) {
                    m_userName = options[sc_userName];
                }
                if (options.find(sc_password) != options.end()) {
                    m_password = options[sc_password];
                    m_havePassword = true;
                }

                if (options.find(sc_tables) != options.end()) {
                    ProcessTables(options[sc_tables]);
                }

                // either we don't need a password or we have one specified
                if (m_passwordRequired == false || m_havePassword == true) {
                    m_connectionString = GetConnectionString();

                    GetLogger().information("Connecting to database using: %s", m_connectionString);

                    try {
                        m_pDBSession = std::make_shared<Poco::Data::Session>(m_databaseTypeName, m_connectionString);

                        success = m_pDBSession->isConnected();

                        if (success == true) {
                            success = SetupDatabase();
                        }

                        SetIsReady(success);
                    }
                    catch (Poco::Exception &excep) {
                        GetLogger().error("Unable to initialize database: %s", std::string(excep.what()));
                        GetLogger().error("Unable to initialize database: %s", excep.message());
                    }
                }
            }
            return success;
        }

        bool DatabaseContainer::WritePacket(const data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            return success;
        }

        bool DatabaseContainer::ReadPacket(data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            return success;
        }

        void DatabaseContainer::ProcessTables(const nlohmann::json &optionSet)
        {
            std::string tableName;

            if (optionSet.is_array() == true) {
                GetLogger().information("Processing tables.");
                for (nlohmann::json::const_iterator iter = optionSet.begin(); iter != optionSet.end(); iter++) {
                    if (iter->is_object() == true) {
                        for (nlohmann::json::const_iterator table = iter->begin(); table != iter->end(); table++) {
                            database::ITableSPtr pTable = database::TableFactory::CreateTable(GetTableType());

                            pTable->Initialize(table.key(), *table);

                            m_tables[table.key()] = pTable;
                        }
                    }
                }
            } else {
                GetLogger().error("Cannot process a non-array");
            }
        }
    }
}
