/*--------------------------------------
  FileContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_FILECONTAINER
#define _H_FILECONTAINER

#include <fstream>

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
        /**
         * @class FileContainer
         * 
         * @brief base implementation for FileContainers
         */
        class FileContainer : public Container
        {
            public:
                /**
                 * @brief default constructor
                 */
                FileContainer();

                /**
                 * @brief default descructor
                 */
                virtual ~FileContainer();

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

            private:
                std::fstream m_fileStream;
        };
    }
}
#endif