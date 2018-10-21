/*--------------------------------------
  Container
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include "ContainerFactory.h"
#include "Container.h"

namespace afm
{
    namespace container
    {
        Container::Container()
            : m_logger(Poco::Logger::get("Container"))
        {

        }

        Container::~Container()
        {
            m_pNotificationRecipient = nullptr;
        }

        bool Container::Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient)
        {
            bool success = false;

            m_pNotificationRecipient = pNotificationRecipient;

            if (options.find(sc_id) != options.end()) {
                m_id = options[sc_id];
            }
            if (options.find(sc_target) != options.end()) {
                m_target = options[sc_target];
                success = true;
            }
            if (options.find(sc_type) != options.end()) {
                m_type = ContainerFactory::GetContainerType(options[sc_type]);
            }
            if (success == false) {
                m_logger.error("Target not specified in container.");
            }

            return success;
        }

        void Container::Shutdown()
        {
            m_pNotificationRecipient = nullptr;
        }

        bool Container::WritePacket(const data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            return success;
        }

        bool Container::ReadPacket(data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            if (m_incomingPackets.empty() == false) {
                std::lock_guard<std::mutex> guard(m_dataMutex);

                data::IDataPacketSPtr pIncomingPacket = m_incomingPackets.front();

                pPacket->Clone(pIncomingPacket);

                m_incomingPackets.pop_front();

                success = true;
            }

            return success;
        }

        bool Container::IsReady() const
        {
            return m_isReady;
        }

        void Container::SetIsReady(bool isReady)
        {
            m_isReady = isReady;

            if (m_pNotificationRecipient != nullptr) {
                m_pNotificationRecipient->OnContainerReady(m_id, m_isReady);
            }
        }

        bool Container::OnPacketWritten(const data::IDataPacketSPtr pPacket, bool success)
        {
            bool wasHandled = false;

            if (m_pNotificationRecipient != nullptr) {
                m_pNotificationRecipient->OnPacketWritten(m_id, pPacket, success);
                wasHandled = true;
            }

            return wasHandled;
        }

        bool Container::OnPacketRead(data::IDataPacketSPtr pPacket, bool success)
        {
            bool wasHandled = false;

            if (m_pNotificationRecipient != nullptr) {                
                m_pNotificationRecipient->OnPacketRead(m_id, pPacket, success);
                wasHandled = true;
            } else if (success == true) {
                // no one directly interested so push it back for later
                std::lock_guard<std::mutex> guard(m_dataMutex);

                m_incomingPackets.push_back(pPacket);
            }

            return wasHandled;
        }
    }
}
