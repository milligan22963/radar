/*--------------------------------------
  BluetoothContainer
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <unistd.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include "data/DataPacketFactory.h"
#include "BluetoothContainer.h"
#include "SystemDefines.h"

namespace afm
{
    namespace container
    {
        BluetoothContainer::BluetoothContainer()
            : Container()
            , m_scanningEnabled(false)
        {

        }

        BluetoothContainer::~BluetoothContainer()
        {
            StopScanning();

            if (m_hciDeviceSocket != NO_HCI_DEVICE) {
                ::close(m_hciDeviceSocket);
                m_hciDeviceSocket = NO_HCI_DEVICE;
            }
        }

        bool BluetoothContainer::Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient)
        {
            bool success = Container::Initialize(options, pNotificationRecipient);

            if (options.find(sc_hciInstance) != options.end()) {
                m_hciInstance = options[sc_hciInstance].get<uint32_t>();

                m_hciDeviceSocket = socket(AF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC, common::Bluetooth_ProtocolLayer::BT_HCI);

                if (m_hciDeviceSocket >= 0) {
                    common::HCI_SockAddr hciAddr;

                    memset(&hciAddr, 0, sizeof(hciAddr));
                    hciAddr.hci_family = AF_BLUETOOTH;
                    hciAddr.hci_device = m_hciInstance;
                //	hciAddr.hci_channel = 0x01; // user channel not available in linux 3.10

                    if (bind(m_hciDeviceSocket, (struct sockaddr *)&hciAddr, sizeof(hciAddr)) == 0) {

                        success = true;
                        
                        int32_t sockOptions = 1;

                        if (setsockopt(m_hciDeviceSocket, common::SocketLevelHCI, common::HCI_SocketOption::HCI_DataDirection, &sockOptions, sizeof(sockOptions)) < 0) {
                            success = false;
                            GetLogger().error("Unable to set socket option, DataDirection");
                        }

                        sockOptions = 1;
                        if (setsockopt(m_hciDeviceSocket, common::SocketLevelHCI, common::HCI_SocketOption::HCI_TimeStamp, &sockOptions, sizeof(sockOptions)) < 0) {
                            success = false;
                            GetLogger().error("Unable to set socket option, TimeStamp");
                        }

                        StartScanning();
                    } else {
                        GetLogger().error("Cannot bind to bluetooth socket");
                    }
                } else {
                    GetLogger().error("Cannot open bluetooth socket");
                }
            } else {
                success = false; // no hci instance
            }
            SetIsReady(success);

            return success;
        }

        void BluetoothContainer::Shutdown()
        {
            StopScanning();

            if (m_hciDeviceSocket != NO_HCI_DEVICE) {
                ::close(m_hciDeviceSocket);
                m_hciDeviceSocket = NO_HCI_DEVICE;
            }
            
            Container::Shutdown();
        }

        bool BluetoothContainer::WritePacket(const data::IDataPacketSPtr pPacket)
        {
            return false;
        }

        bool BluetoothContainer::ReadPacket(data::IDataPacketSPtr pPacket)
        {
            return false;
        }

        void BluetoothContainer::StartScanning()
        {
            m_eventFilter.AssociateSocket(m_hciDeviceSocket);

            // We need to enable the reception of commands and status
            // we want to set some values
            m_eventFilter.SetPacketType(common::HCI_PacketType::HCI_EventPacket);
            m_eventFilter.SetControllerCommand(common::LE_ControllerCommand::LE_SetScanParameters);
            m_eventFilter.EnableMask(common::HCI_EventType::HCI_LEMetaEvent);
            m_eventFilter.EnableMask(common::HCI_EventType::HCI_CommandComplete);
            m_eventFilter.EnableMask(common::HCI_EventType::HCI_CommandStatus);

            if (m_eventFilter.ApplyEventMask() == false) {
                GetLogger().error("Applying event mask for set scan parameters");
            }

            common::LE_ScanParams scanParams;

            memset(&scanParams, 0, sizeof(scanParams));

            scanParams.type = 0; // 0 passive, 1 active
            scanParams.interval = 0x1000;
            scanParams.window = scanParams.interval;
            scanParams.filterDuplicates = 0;
            scanParams.localAddressType = 0; // public

            common::HCI_Command command;
            command.commandCode = common::LE_ControllerCommand::LE_SetScanParameters;

            // send scan params
            command.parameterLength = sizeof(scanParams);
            command.pParameters = &scanParams;

            if (SendCommand(command) == true) {
                m_eventFilter.Reset();

                // enable scanning
                common::LE_ScanEnableParams scanEnable;

                scanEnable.enable = 1;
                scanEnable.filterDuplicates = 0;

                m_eventFilter.SetControllerCommand(common::LE_ControllerCommand::LE_SetScanEnable);
                if (m_eventFilter.ApplyEventMask() == false) {
                    GetLogger().error("Applying event mask for set scan enable");
                }

                command.commandCode = common::LE_ControllerCommand::LE_SetScanEnable;

                command.parameterLength = sizeof(scanEnable);
                command.pParameters = &scanEnable;

                if (SendCommand(command) == true) {
                    m_eventFilter.Reset();

                    m_scanningEnabled = true;

                    GetLogger().information("Scanning has been enabled, now to receive data");

                    // Kick off the receive thread
                    m_receiveThread = std::thread(&BluetoothContainer::ReceiveBluetoothData, this);
                } else {
                    GetLogger().error("Failed to send command set scan enable");
                }
            } else {
                GetLogger().error("Failed to send command set scan parameters");
            }
        }

        void BluetoothContainer::StopScanning()
        {
            if (m_scanningEnabled == true) {
                m_scanningEnabled = false;

                m_receiveThread.join();

				m_eventFilter.SetPacketType(common::HCI_PacketType::HCI_EventPacket);
                m_eventFilter.EnableMask(common::HCI_EventType::HCI_CommandComplete);
                m_eventFilter.EnableMask(common::HCI_EventType::HCI_CommandStatus);
                m_eventFilter.SetControllerCommand(common::LE_ControllerCommand::LE_SetScanEnable);
				if (m_eventFilter.ApplyEventMask() == false) {
				}

                // disable scan
                common::LE_ScanEnableParams scanEnable;

				scanEnable.enable = 0; // disable it
				scanEnable.filterDuplicates = 0;

                common::HCI_Command command;

				command.commandCode = common::LE_ControllerCommand::LE_SetScanEnable;

				command.parameterLength = sizeof(scanEnable);
				command.pParameters = &scanEnable;
				if (SendCommand(command) == true) {
                    m_eventFilter.Reset();
				}
			}
        }

		bool BluetoothContainer::SendCommand(const common::HCI_Command &command)
		{
			bool success = false;

            common::HCI_PacketType packetType = common::HCI_PacketType::HCI_CommandPacket;

            struct iovec commandData[common::HCI_CommandFields::NUM_HCI_CommandFields];
            int commandDataEntries = common::HCI_CommandFields::NUM_HCI_CommandFields;

            memset(&commandData, 0, sizeof(struct iovec) * commandDataEntries);

            commandData[common::HCI_CommandFields::HCI_PacketTypeField].iov_base = &packetType;
            commandData[common::HCI_CommandFields::HCI_PacketTypeField].iov_len = sizeof(common::HCI_PacketType);

            commandData[common::HCI_CommandFields::HCI_CommandStructure].iov_base = (void *)&command;
            commandData[common::HCI_CommandFields::HCI_CommandStructure].iov_len = common::HCI_CommandSize;

            if (command.parameterLength > 0) {
                commandData[common::HCI_CommandFields::HCI_ParameterFields].iov_base = command.pParameters;
                commandData[common::HCI_CommandFields::HCI_ParameterFields].iov_len = command.parameterLength;
            } else {
                commandDataEntries--; // no params deduct one
            }

            success = true;
            while(writev(m_hciDeviceSocket, commandData, commandDataEntries) < 0) {
                if ((errno == EAGAIN) || (errno == EINTR)) {
                    continue;
                }
                GetLogger().error("Failed to write command out.");
                success = false;
                break;
            }

            if (success == true) {
                common::HCI_Event response;

                success = ReceiveResponse(response);

                if (response.pParams != nullptr) {
                    delete [] (uint8_t *)(response.pParams);
                }
            }
			return success;
		}

		bool BluetoothContainer::ReceiveResponse(common::HCI_Event &response)
		{
			bool success = false;

            response.pParams = nullptr;
            response.parameterLength = 0;

            // defined but not currently used in this function
            struct timespec timeStamp;
            uint8_t tempBuffer[512];

            if (ReceiveData(tempBuffer, 512, 5000, timeStamp) > 0) {
                success = true;
                if (response.parameterLength > 0) {
                    response.pParams = new uint8_t[response.parameterLength];
                    memcpy(response.pParams, &tempBuffer[common::HCI_EventSize], response.parameterLength);
                }
            } else {
                GetLogger().error("Timed out waiting for data.");
            }
			return success;
		}

		int BluetoothContainer::ReceiveData(uint8_t *pBuffer, uint16_t maxLength, uint32_t timeOutMS, struct timespec &timeStamp)
		{
			int bytesRead = 0;

			fd_set readDescriptorSet;
			struct timeval tv;

			memset(&timeStamp, 0, sizeof(timeStamp));

			FD_ZERO(&readDescriptorSet);
			FD_SET(m_hciDeviceSocket, &readDescriptorSet);

			tv.tv_sec = timeOutMS / 1000;
			tv.tv_usec = timeOutMS % 1000;

			if (select(m_hciDeviceSocket + 1, &readDescriptorSet, NULL, NULL, &tv) != -1) {
				if (FD_ISSET(m_hciDeviceSocket, &readDescriptorSet)) {
					if (clock_gettime(CLOCK_REALTIME, &timeStamp) == -1) {
                        GetLogger().error("Failed to get time stamp in received data");
					}
					bytesRead = read(m_hciDeviceSocket, pBuffer, maxLength);
				}
			}
			return bytesRead;
		}

        void BluetoothContainer::ReceiveBluetoothData()
        {
			uint16_t scratchBufferLength = 1024;
			uint8_t *pScratchBuffer = new uint8_t[scratchBufferLength];
			struct timespec timeStamp;

            GetLogger().information("Starting receive thread");

            m_eventFilter.SetPacketType(common::HCI_PacketType::HCI_EventPacket);
            m_eventFilter.SetControllerCommand(common::LE_ControllerCommand::LE_SetEventMask);
            m_eventFilter.EnableMask(common::HCI_EventType::HCI_LEMetaEvent);
            if (m_eventFilter.ApplyEventMask() == false) {
                GetLogger().error("Unable to apply receive filter");
            }

            while (m_scanningEnabled == true) {                 
                int dataReceived = ReceiveData(pScratchBuffer, scratchBufferLength, 1000, timeStamp);
                if (dataReceived > 0) {
                    data::IDataPacketSPtr pPacket = createPacket(pScratchBuffer, dataReceived);

                    if (pPacket != nullptr) {
                        if (pPacket->HasMultiplePackets() == true) {
                            std::string timeValue;
                            
                            // all packets should have the same timestamp
                            pPacket->GetValue(sc_timeStamp, timeValue);

                            data::IDataPacketSPtr pChildPacket = pPacket->getNextPacket();
                            while (pChildPacket != nullptr) {
                                // override the base timestamp
                                pChildPacket->SetValue(sc_timeStamp, timeValue);
                                OnPacketRead(pChildPacket, true);
                                pChildPacket = pPacket->getNextPacket();
                            }
                        } else {
                            // only one, receive it
                            OnPacketRead(pPacket, true);
                        }
                    }
                }
            }

            m_eventFilter.Reset();

            delete [] pScratchBuffer;

            GetLogger().information("Finishing receive thread");
        }

        data::IDataPacketSPtr BluetoothContainer::createPacket(uint8_t *pData, int dataLength)
        {
            data::IDataPacketSPtr pPacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::BLUETOOTH);

            data::RawData incomingMessage;

            incomingMessage.insert(incomingMessage.begin(), (char *)pData, (char *)(pData + dataLength));

            if (pPacket->FromRaw(incomingMessage) == false) {
                pPacket = nullptr; // cant parse so null it
            }

            return pPacket;
        }
    }
}