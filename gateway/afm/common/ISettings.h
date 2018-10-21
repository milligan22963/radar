/*--------------------------------------
  ISettings
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_ISETTINGS
#define _H_ISETTINGS

#include <map>
#include <memory>
#include <string>

#include "container/IContainerNotification.h"
#include "container/IContainer.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm 
{
    /**
     * @brief secondary level for common definitions
     */
    namespace common
    {
        using ContainerMap = std::map<std::string, container::IContainerSPtr>;

        /**
         * @interface ISettings
         * 
         * Describes the ISettings interface for loading application
         * configuration
         */
        class ISettings
        {
            public:
                /**
                 * @brief Standard virtual destructor.
                 */
                virtual ~ISettings() {}

                /**
                 * @brief initializes the settings based on the passed in
                 *          configuration file and optional listener.
                 * 
                 * @param[in] configurationFile - the name of the file (full path) of the configuration data
                 * @param[in] pNotificationRecipient - the received of data packets read/written and container
                 *              readiness
                 * 
                 * @return true on success, false otherwise
                 */
                virtual bool Initialize(const std::string &configurationFile, container::IContainerNotificationSPtr pNotificationRecipient = nullptr) = 0;

                /**
                 * @brief shuts down the setting manager and closes out all containers
                 */
                virtual void Shutdown() = 0;

                /**
                 * @brief returns all of the containers created based on the configuration file
                 * 
                 * @return the ContainerMap of containers
                 */
                virtual ContainerMap &GetContainers() = 0;

                /**
                 * @brief returns the specific container based on the id as specified in the configuration file
                 * 
                 * @param[in] containerId - the id of the container to find/return
                 * 
                 * @return IContainerSPtr - the shared pointer to the container if found
                 *                          nullptr otherwise
                 */
                virtual container::IContainerSPtr GetContainer(const std::string &containerId) = 0;
        };

        using ISettingsSPtr = std::shared_ptr<ISettings>;
    }
}

#endif
