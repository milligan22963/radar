/*--------------------------------------
  IContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_ICONTAINER
#define _H_ICONTAINER

#include <string>
#include <nlohmann/json.hpp>

#include "IContainerNotification.h"
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
         * Well known fields
         */
        const std::string sc_type = "type";
        const std::string sc_target = "target";
        const std::string sc_id = "id";

        using ContainerOptions = nlohmann::json;

        /**
         * @class IContainer
         * 
         * @brief abstract implementation for containers
         */
        class IContainer
        {
            public:
                /**
                 * @brief default descructor
                 */
                virtual ~IContainer() {};

                /**
                 * @brief initializes a container with the given options
                 * 
                 * @param[in] options - the options for the given container
                 * @param[in] pNotificationRecipient - someone interested in container
                 *              related events
                 * 
                 * @return bool - true if successful, false otherwise
                 */
                virtual bool Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient = nullptr) = 0;

                /**
                 * @brief shuts down a container when no longer needed
                 */
                virtual void Shutdown() = 0;

                /**
                 * @brief writes a packet to the given container
                 * 
                 * @param[in] pPacket - the packet to write
                 * 
                 * @return bool - true if successful, false otherwise
                 */
                virtual bool WritePacket(const data::IDataPacketSPtr pPacket) = 0;

                /**
                 * @brief reads a packet from the given container
                 * 
                 * @param[in] pPacket - the packet to read
                 * 
                 * @return bool - true if successful, false otherwise
                 */
                virtual bool ReadPacket(data::IDataPacketSPtr pPacket) = 0;

                /**
                 * @brief determines if the container is ready or not
                 *
                 * @return boolean with true indicating it is ready, false otherwise
                 */
                virtual bool IsReady() const = 0;
        };

        using IContainerSPtr = std::shared_ptr<IContainer>;
    }
}
#endif