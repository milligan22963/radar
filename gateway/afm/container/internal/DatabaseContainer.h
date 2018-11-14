/*--------------------------------------
  DatabaseContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_DATABASE_CONTAINER
#define _H_DATABASE_CONTAINER

#include <map>
#include <string>

#include <Poco/Data/Session.h>
#include "database/ITable.h"
#include "database/TableFactory.h"
#include "Container.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level namespace for container library assets
     */
    namespace container
    {
        /**
         * @class DatabaseContainer
         * 
         * @brief base implementation for database containers
         */
        class DatabaseContainer : public Container
        {
            public:
                /**
                 * @brief default constructor
                 */
                DatabaseContainer();

                /**
                 * @brief default descructor
                 */
                virtual ~DatabaseContainer();

                /**
                 * @copydoc IContainer::Initialize
                 */
                virtual bool Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient = nullptr) override;

                /**
                 * @copydoc IContainer::WritePacket
                 */
                virtual bool WritePacket(const data::IDataPacketSPtr pPacket) override;

                /**
                 * @copydoc IContainer::ReadPacket
                 */
                virtual bool ReadPacket(data::IDataPacketSPtr pPacket) override;

            protected:
                virtual std::string GetConnectionString() const = 0;
                virtual bool SetupDatabase() = 0;

                void SetDatabaseTypeName(const std::string &dbTypeName) { m_databaseTypeName = dbTypeName; }
                void SetPasswordRequired(bool isRequired) { m_passwordRequired = isRequired; }

                std::string GetDatabaseName() const { return GetTarget(); }
                std::string GetUserName() const { return m_userName; }
                std::string GetPassword() const { return m_password; }
                std::string GetHostName() const { return m_hostName; }
                uint32_t GetPort() const { return m_port; }
                void ProcessTables(const nlohmann::json &optionSet);
                const database::Tables &GetTables() const { return m_tables; }
                void SetTableType(database::TableType type) { m_tableType = type; }
                database::TableType GetTableType() const { return m_tableType; }

            private:
                database::TableType m_tableType = database::TableType::SLQLITE;
                std::string m_databaseTypeName = "SQLite"; // default
                std::string m_userName = "afm";
                std::string m_password = "afm";
                std::string m_hostName = "localhost";
                uint32_t m_port = 3306;
                bool m_passwordRequired = false;
                bool m_havePassword = false;

                std::shared_ptr<Poco::Data::Session> m_pDBSession = nullptr;

                std::string m_connectionString;
                database::Tables m_tables;
        };
    }
}
#endif