/*--------------------------------------
  Settings
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_SETTINGS
#define _H_SETTINGS

#include <Poco/Logger.h>

#include "ISettings.h"

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
        /**
         * @class Settings
         * 
         * Describes the Settings base class for loading application
         * configuration
         */
        class Settings : public ISettings
        {
            public:
                /**
                 * Standard constructor
                 */
                Settings();

                /**
                 * Standard destructor
                 */
                virtual ~Settings();

                /**
                 * @copydoc ISettings;:Initialize
                 */
                virtual bool Initialize(const std::string &configurationFile, container::IContainerNotificationSPtr pNotificationRecipient = nullptr) override;

                /**
                 * @copydoc ISettings;:GetContainers
                 */
                virtual ContainerMap &GetContainers() override
                {
                    return m_containers;
                }

                /**
                 * @copydoc ISettings;:GetContainer
                 */
                virtual container::IContainerSPtr GetContainer(const std::string &containerId) override;

                /**
                 * @copydoc ISettings;:Shutdown
                 */
                virtual void Shutdown() override;

            private:
                ContainerMap m_containers;

                Poco::Logger &m_logger;
        };
    }
}

#endif
