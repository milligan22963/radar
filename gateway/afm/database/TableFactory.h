/*--------------------------------------
  TableFactory

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_TABLEFACTORY
#define _H_TABLEFACTORY

#include <Poco/Logger.h>
#include "ITable.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level namespace for database library assets
     */
    namespace database
    {
        enum class TableType
        {
            SLQLITE,
            MYSQL, // including maria
            POSTGRES,
            END_TABLE_TYPES
        };

        /**
         * @class Table
         * 
         * Defines the basic table interface
         */
        class TableFactory
        {
            public:
                static ITableSPtr CreateTable(TableType type);
        };
    }
}

#endif