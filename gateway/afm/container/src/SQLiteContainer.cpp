/*--------------------------------------
  SQLiteContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <Poco/Data/SQLite/Connector.h>

#include "SQLiteContainer.h"

namespace afm
{
    namespace container
    {
        bool SQLiteContainer::m_isRegistered = false;

        SQLiteContainer::SQLiteContainer()
        {
            SetDatabaseTypeName("SQLite");
            SetPasswordRequired(false);

            // must have this registered
            if (m_isRegistered == false) {
                Poco::Data::SQLite::Connector::registerConnector();
                m_isRegistered = true;
            }
        }

        SQLiteContainer::~SQLiteContainer()
        {

        }

        bool SQLiteContainer::WritePacket(const data::IDataPacketSPtr pPacket)
        {
            return false;
        }

        bool SQLiteContainer::ReadPacket(data::IDataPacketSPtr pPacket)
        {
            return false;
        }

        // protected
        std::string SQLiteContainer::GetConnectionString() const
        {
            return GetDatabaseName(); // for SQLite, the target is the database name
        }

        bool SQLiteContainer::SetupDatabase()
        {
            for (auto table : GetTables()) {
                // Verify table exists, if not create it
            }
            return true;
        }
    }
}
