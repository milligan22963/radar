/*--------------------------------------
  BluetoothContainer
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_BLUETOOTHCONTAINER
#define _H_BLUETOOTHCONTAINER

#include <atomic>
#include <thread>

#include "common/BluetoothHCI.h"

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
         * Well known fields for options
         */
        const std::string sc_hciInstance = "hci";

        /**
         * @class BluetoothContainer
         * 
         * @brief base implementation for BluetoothContainer
         */
        class BluetoothContainer : public Container
        {
            public:
                /**
                 * @brief default constructor
                 */
                BluetoothContainer();

                /**
                 * @brief default descructor
                 */
                virtual ~BluetoothContainer();

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

            protected:

            private:
                void StartScanning();
                void StopScanning();
                bool SendCommand(const common::HCI_Command &command);
		        bool ReceiveResponse(common::HCI_Event &response);
		        int ReceiveData(uint8_t *pBuffer, uint16_t maxLength, uint32_t timeOutMS, struct timespec &timeStamp);

                void ReceiveBluetoothData();

                data::IDataPacketSPtr createPacket(uint8_t *pData, int dataLength);

            private:
                common::BluetoothHCIFilter m_eventFilter;

                const int32_t NO_HCI_DEVICE = -1;

                uint32_t m_hciInstance = 0;
                int32_t m_hciDeviceSocket = NO_HCI_DEVICE; // socket handle
                std::atomic<bool> m_scanningEnabled;

                std::thread m_receiveThread;
        };
    }
}
#endif