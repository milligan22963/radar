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
        ITableSPtr TableFactory::CreateTable(std::string type)
        {
            ITableSPtr pTable = std::make_shared<Table>();

            return pTable;
        }
    }
}
