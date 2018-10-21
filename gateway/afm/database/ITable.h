/*--------------------------------------
  ITable

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_ITABLE
#define _H_ITABLE

#include <map>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

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
         * @brief TableOptions
         * 
         * Table Options for where clauses name=value
         */
        using TableOptions = std::map<std::string, std::string>;

        /**
         * @interface ITable
         * 
         * Defines the basic table interface
         */
        class ITable 
        {
            public:
                virtual ~ITable() {}

                virtual bool Initialize(const std::string &tableName, const nlohmann::json &tableInformation) = 0;
        };

        using ITableSPtr = std::shared_ptr<ITable>;
    }
}

#endif