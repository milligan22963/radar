/*--------------------------------------
  Column

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <sstream>
#include "Column.h"

namespace afm
{
    namespace database
    {
        static const std::string sc_columnType = "type";
        static const std::string sc_columnLength = "length";
        static const std::string sc_columnOptions = "options";

        /**
         * @note
         *  The valid types are...
         */
        static const std::string sc_integer = "integer";

        Column::Column()
            : m_logger(Poco::Logger::get("DBColumn"))
        {

        }

        Column::~Column()
        {

        }

        bool Column::Initialize(const std::string &columnName, const nlohmann::json &details)
        {
            bool success = false;

            m_columnName = columnName;

/*					"id": {
						"type": "integer",
						"options": [
							"primary",
							"key"
						]
					},*/
            // pull out the type and the options if any
            if (details.find(sc_columnType) != details.end()) {
                m_columnType = details[sc_columnType].get<std::string>();
                m_logger.information("Type: %s", details[sc_columnType].get<std::string>());
                success = true; // must have at least a type
            }

            if (details.find(sc_columnLength) != details.end()) {
                // get length
            }

            if (details.find(sc_columnOptions) != details.end()) {
                
            }

            return success;
        }

        std::string Column::ToString() const
        {
            std::stringstream representation;

            representation << m_columnName;

            return representation.str();
        }
    }
}
