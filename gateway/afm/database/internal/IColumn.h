/*--------------------------------------
  IColumn

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_ICOLUMN
#define _H_ICOLUMN

#include <memory>
#include <string>

#include <json.hpp>

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

        enum ColumnType
        {
            colTypeInteger,
            colTypeTimeStamp,
            endColType
        };

        /**
         * @interface IColumn
         * 
         * Defines the basic column interface
         */
        class IColumn
        {
            public:
                virtual ~IColumn() {}

                virtual bool Initialize(const std::string &columnName, const nlohmann::json &details) = 0;
                virtual std::string ToString() const = 0;
        };

        using IColumnSPtr = std::shared_ptr<IColumn>;
    }
}

#endif