/*--------------------------------------
  Radar

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <memory>

#include "application/AFMApp.h"
#include "container/ContainerFactory.h"
#include "container/IContainerNotification.h"

namespace afm
{
    namespace application
    {
        class Radar : public AFMApp
        {
            public:
                Radar();

                /**
                 * @copydoc IContainerNotification::OnPacketWritten
                 */
                virtual void OnPacketWritten(const std::string &containerId, const data::IDataPacketSPtr pPacket, bool success) override;

                /**
                 * @copydoc IContainerNotification::OnPacketRead
                 */
                virtual void OnPacketRead(const std::string &containerId, data::IDataPacketSPtr pPacket, bool success) override;

                /**
                 * @copydoc IContainerNotification::OnContainerReady
                 */
                virtual void OnContainerReady(const std::string &containerId, bool isReady) override;

            protected:
                /**
                 * @copydoc AFMApp::OnStartup
                 */
                virtual void OnStartup() override;

                /**
                 * @copydoc AFMApp::OnShutdown
                 */
                virtual void OnShutdown() override;

            private:
                container::IContainerSPtr m_receiver = nullptr;
                container::IContainerSPtr m_offload = nullptr;
        };
    }
}