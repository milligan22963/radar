/*--------------------------------------
  Column

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_COLUMN
#define _H_COLUMN

#include <vector>
#include <string>

#include <Poco/Logger.h>

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
         * @interface Column
         * 
         * Defines the basic column interface
         */
        class Column : public IColumn
        {
            public:
                Column();
                virtual ~Column();

                virtual bool Initialize(const std::string &columnName, const nlohmann::json &details) override;
                virtual std::string ToString() const;

            private:
                std::string m_columnName;
                std::string m_columnType;
                uint32_t m_length = 0;
                std::vector<std::string> m_options;
                Poco::Logger &m_logger;
        };
    }
}

#endif