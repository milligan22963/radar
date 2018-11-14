/*--------------------------------------
  TableFactory

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include "TableFactory.h"

#include "Table.h"

namespace afm
{
    namespace database
    {
        ITableSPtr TableFactory::CreateTable(TableType type)
        {
            // right now we have one table, type will be db specific
            ITableSPtr pTable = std::make_shared<Table>();

            return pTable;
        }
    }
}
