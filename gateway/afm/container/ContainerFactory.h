/*--------------------------------------
  ContainerFactory
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_CONTAINERFACTORY
#define _H_CONTAINERFACTORY

#include <memory>
#include "IContainer.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level namespace for container library assets
     */
    namespace container
    {
        enum ContainerType
        {
            EMPTY_CONTAINER,
            FILE_CONTAINER,
            HTTP_CONTAINER,
            HTTPS_CONTAINER,
            MQTT_CONTAINER,
            BLUETOOTH_CONTAINER,
            SQLITE_DATABASE_CONTAINER,
            END_CONTAINER_TYPES
        };

        /**
         * @class ContainerFactory
         * 
         * @brief base implementation for ContainerFactory
         */
        class ContainerFactory
        {
            public:
                static IContainerSPtr CreateContainer(ContainerType type);
                static ContainerType GetContainerType(const std::string &containerName);

            private:
                /**
                 * @brief default constructor
                 */
                ContainerFactory();

                /**
                 * @brief default descructor
                 */
                virtual ~ContainerFactory();
        };
    }
}
#endif