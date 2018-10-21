/*--------------------------------------
  DataPacketFactory
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_DATAPACKETFACTORY
#define _H_DATAPACKETFACTORY

#include <memory>
#include "IDataPacket.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level namespace for data library assets
     */
    namespace data
    {
        enum DataPacketType
        {
            DATA,
            BLUETOOTH,
            END_DATA_TYPES
        };

        /**
         * @class DataPacketFactory
         * 
         * @brief base implementation for DataPacketFactory
         */
        class DataPacketFactory
        {
            public:
                static IDataPacketSPtr CreateDataPacket(DataPacketType type);

            private:
                /**
                 * @brief default constructor
                 */
                DataPacketFactory();

                /**
                 * @brief default descructor
                 */
                virtual ~DataPacketFactory();
        };
    }
}
#endif