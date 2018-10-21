/*--------------------------------------
  FileContainer
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>

#include "FileContainer.h"

namespace afm
{
    namespace container
    {
        FileContainer::FileContainer()
        {

        }

        FileContainer::~FileContainer()
        {

        }

        bool FileContainer::Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient)
        {
            // do this first to get target and all options
            bool success = Container::Initialize(options, pNotificationRecipient);

            if (success == true) {
                m_fileStream.open(GetTarget(), std::ios::in | std::ios::out | std::ios::app);

                success = m_fileStream.is_open();

                SetIsReady(success);
            }

            if (success == false) {
                GetLogger().error("Unable to initialize File container");
            }

            return success;
        }

        bool FileContainer::WritePacket(const data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            if (m_fileStream.is_open() == true) {
                m_fileStream << pPacket->ToJSON() << "\n";
                success = true;
            }
            return success;
        }

        bool FileContainer::ReadPacket(data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            if (m_fileStream.is_open()) {
                // read json data from file
                std::string incomingData;

                if (m_fileStream.eof() == false) {
                    std::getline(m_fileStream, incomingData);

                    if (m_fileStream.fail() == false) {
                        pPacket->FromJSON(incomingData);

                        success = true;
                    }
                }
            }

            if (success == false) {
                SetIsReady(false);
            }

            return success;
        }
    }
}