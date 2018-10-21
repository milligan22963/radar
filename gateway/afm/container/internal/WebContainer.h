/*--------------------------------------
  WebContainer
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_WEBCONTAINER
#define _H_WEBCONTAINER

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/URI.h>
#include <deque>

#include "Container.h"

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
        using HTTPClientSessionSPtr = std::shared_ptr<Poco::Net::HTTPClientSession>;

        /**
         * @class WebContainer
         * 
         * @brief base implementation for WebContainers
         */
        class WebContainer : public Container
        {
            public:
                /**
                 * @brief default constructor
                 */
                WebContainer();

                /**
                 * @brief default descructor
                 */
                virtual ~WebContainer();

                /**
                 * @copydoc IContainer::Initialize
                 */
                virtual bool Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient = nullptr) override;

                /**
                 * @copydoc IContainer::WritePacket
                 */
                virtual bool WritePacket(const data::IDataPacketSPtr pPacket) override;

                /**
                 * @copydoc IContainer::ReadPacket
                 */
                virtual bool ReadPacket(data::IDataPacketSPtr pPacket) override;

            protected:
                /**
                 * @brief returns the active client session for this container
                 * 
                 * @return the active client session
                 */
                HTTPClientSessionSPtr GetClientSession() { return m_pClientSession; }

                /**
                 * @brief creates the active client session to use for this container
                 * 
                 * @param[in] uri the uri for this client session
                 * 
                 * @return clientSession - shared pointer to a client session
                 */
                virtual HTTPClientSessionSPtr CreateClientSession(const Poco::URI &uri);

            private:
                HTTPClientSessionSPtr m_pClientSession = nullptr;
                std::string m_targetPath;
                data::IDataPacketSPtr m_responsePacket = nullptr;
        };
    }
}
#endif