/*--------------------------------------
  Container

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_CONTAINER
#define _H_CONTAINER

#include <mutex>
#include <Poco/Logger.h>

#include "IContainer.h"
#include "ContainerFactory.h"

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
         * @class Container
         * 
         * @brief base implementation for containers
         */
        class Container : public IContainer
        {
            public:
                /**
                 * @brief default constructor
                 */
                Container();

                /**
                 * @brief default descructor
                 */
                virtual ~Container();

                /**
                 * @copydoc IContainer::Initialize
                 */
                virtual bool Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient = nullptr) override;

                /**
                 * @copydoc IContainer::Shutdown
                 */
                virtual void Shutdown() override;

                /**
                 * @copydoc IContainer::WritePacket
                 */
                virtual bool WritePacket(const data::IDataPacketSPtr pPacket) override;

                /**
                 * @copydoc IContainer::ReadPacket
                 */
                virtual bool ReadPacket(data::IDataPacketSPtr pPacket) override;

                /**
                 * @copydoc IContainer::IsReady
                 */
                virtual bool IsReady() const final;

            protected:
                /**
                 * @brief returns the given target for this container
                 * 
                 * @return std::string target such as /tmp/filename, http://localhost, etc.
                 */
                std::string GetTarget() const { return m_target; }

                /**
                 * @brief returns a collection of options for this container
                 * 
                 * @return the collection of options
                 */
                const ContainerOptions &GetOptions() const { return m_options; }

                /**
                 * @brief get access to the logger instance for containers
                 *
                 * @return reference to a logger
                 */
                Poco::Logger &GetLogger() const { return m_logger; }

                /**
                 * @brief allows derived classes to set the readiness of this container
                 * 
                 * @param[in] isReady indicating this containers readiness
                 */
                void SetIsReady(bool isReady);

                /**
                 * @brief allows derived classes to indicate a packet was written
                 * 
                 * @param[in] pPacket - the packet written
                 * @param[in] success - true if it was a successful write
                 * 
                 * @return bool true if someone handled it, false otherwise
                 */
                bool OnPacketWritten(const data::IDataPacketSPtr pPacket, bool success);

                /**
                 * @brief notification a packet can be read
                 * 
                 * @param[in] pPacket - the packet to read
                 * @param[in] success - true if it was a successful read
                 * 
                 * @return bool true if someone handled it, false otherwise
                 */
                bool OnPacketRead(data::IDataPacketSPtr pPacket, bool success);

            private:
                bool m_isReady = false;
                std::string m_target;
                std::string m_id = " ";
                ContainerOptions m_options;
                Poco::Logger &m_logger;
                IContainerNotificationSPtr m_pNotificationRecipient = nullptr;
                std::mutex m_dataMutex;
                data::DataPacketDeque m_incomingPackets;
                ContainerType m_type;
        };
    }
}
#endif