/*--------------------------------------
  Table

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include "Column.h"
#include "Table.h"

namespace afm
{
    namespace database
    {
        Table::Table()
            : m_logger(Poco::Logger::get("Table"))
        {

        }

        Table::~Table()
        {
            m_columns.clear();
        }

        bool Table::Initialize(const std::string &tableName, const nlohmann::json &tableInformation)
        {
            bool success = false;

            m_name = tableName;
            m_logger.information("Parsing incoming json data");

            m_logger.information("Table: %s", tableName);

            for (nlohmann::json::const_iterator column = tableInformation.begin(); column != tableInformation.end(); column++) {
                m_logger.information("Column: %s", column.key());
                if (column->is_string()) {
                    m_logger.information("string: %s", column->get<std::string>());
                } else {
                    IColumnSPtr pColumn = std::make_shared<Column>();

                    if (pColumn->Initialize(column.key(), column.value()) == true) {
                        m_logger.information(pColumn->ToString());
                        m_columns.push_back(pColumn);
                    }
                }
            }

            return success;
        }

        bool Table::DoesExist() const
        {
            bool exists = false;

            return exists;
        }

        bool Table::Create() const
        {
            bool success = false;

            return success;
        }
    }
}
