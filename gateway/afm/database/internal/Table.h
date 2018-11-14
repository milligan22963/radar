/*--------------------------------------
  Table

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_TABLE
#define _H_TABLE

#include <vector>
#include <Poco/Logger.h>
#include "ITable.h"
#include "IColumn.h"

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
        /**
         * @class Table
         * 
         * Defines the basic table interface
         */
        class Table : public ITable
        {
            public:
                Table();
                virtual ~Table();

                virtual bool Initialize(const std::string &tableName, const nlohmann::json &tableInformation) override;
                virtual bool DoesExist() const override;
                virtual bool Create() const override;
                virtual std::string Name() const override { return m_name; }

            private:
                std::string m_name;
                std::vector<IColumnSPtr> m_columns;
                Poco::Logger &m_logger;
        };
    }
}

#endif