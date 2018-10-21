/*--------------------------------------
  IContainerNotification

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_INOTIFICATIONCONTAINER
#define _H_INOTIFICATIONCONTAINER

#include <memory>
#include <string>
#include <nlohmann/json.hpp>

#include "data/IDataPacket.h"

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
        /**
         * @class IContainerNotification
         * 
         * @brief abstract implementation for container notifications
         */
        class IContainerNotification
        {
            public:
                /**
                 * @brief default descructor
                 */
                virtual ~IContainerNotification() {};

                /**
                 * @brief notification a packet was written
                 * 
                 * @param[in] containerId - the id of the container making the call
                 * @param[in] pPacket - the packet written
                 * @param[in] success - true if it was a successful write
                 */
                virtual void OnPacketWritten(const std::string &containerId, const data::IDataPacketSPtr pPacket, bool success) = 0;

                /**
                 * @brief notification a packet can be read
                 * 
                 * @param[in] containerId - the id of the container making the call
                 * @param[in] pPacket - the packet to read
                 * @param[in] success - true if it was a successful read
                 */
                virtual void OnPacketRead(const std::string &containerId, data::IDataPacketSPtr pPacket, bool success) = 0;

                /**
                 * @brief notification a container is ready or not
                 *
                 * @param[in] containerId - the id of the container making the call
                 * @param[in] isReady - true if the container is ready, false otherwise
                 */
                virtual void OnContainerReady(const std::string &containerId, bool isReady) = 0;
        };

        using IContainerNotificationSPtr = std::shared_ptr<IContainerNotification>;
    }
}
#endif