/*--------------------------------------
  SQLiteContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_SQLITEDATABASE_CONTAINER
#define _H_SQLITEDATABASE_CONTAINER

#include "DatabaseContainer.h"

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
         * @class SQLiteContainer
         * 
         * @brief sqlite containers
         */
        class SQLiteContainer : public DatabaseContainer
        {
            public:
                /**
                 * @brief default constructor
                 */
                SQLiteContainer();

                /**
                 * @brief default descructor
                 */
                virtual ~SQLiteContainer();

                /**
                 * @copydoc IContainer::WritePacket
                 */
                virtual bool WritePacket(const data::IDataPacketSPtr pPacket) override;

                /**
                 * @copydoc IContainer::ReadPacket
                 */
                virtual bool ReadPacket(data::IDataPacketSPtr pPacket) override;

            protected:
                virtual std::string GetConnectionString() const;
                virtual bool SetupDatabase() override;

            private:
                static bool m_isRegistered;
        };
    }
}
#endif