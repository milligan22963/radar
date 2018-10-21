/*--------------------------------------
  Table

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

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

        }

        bool Table::Initialize(const std::string &tableName, const nlohmann::json &tableInformation)
        {
            bool success = false;

            m_logger.information("Parsing incoming json data");

            m_logger.information("Table: %s", tableName);

            for (nlohmann::json::const_iterator column = tableInformation.begin(); column != tableInformation.end(); column++) {
                m_logger.information("Column: %s", column.key());
                if (column->is_string()) {
                    m_logger.information("string: %s", column->get<std::string>());
                }
            }

            return success;
        }
    }
}
