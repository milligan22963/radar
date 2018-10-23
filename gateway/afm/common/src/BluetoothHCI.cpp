/*--------------------------------------
  BluetoothHCI
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <sys/socket.h>
#include <Poco/Logger.h>

#include "BluetoothHCI.h"

namespace afm
{
    namespace common
    {
        BluetoothHCIFilter::BluetoothHCIFilter()
        {

        }

        BluetoothHCIFilter::~BluetoothHCIFilter()
        {
            Reset();
        }

        void BluetoothHCIFilter::AssociateSocket(int32_t socketInstance)
        {
            if (m_filterApplied == false) {
                m_socketInstance = socketInstance;
            }
        }

        bool BluetoothHCIFilter::ApplyEventMask()
        {
            bool success = false;

            if (getsockopt(m_socketInstance, SocketLevelHCI, HCI_SocketOption::HCI_Filter, m_filterData, &m_filterDataLength) == 0) {
                HCI_FilterParams filter;

                filter.packetTypeMask = m_packetTypeMask;
                filter.eventMask[0] = m_eventMask2[0];
                filter.eventMask[1] = m_eventMask2[1];
                filter.controllerCommand = m_controllerCommand;

               if (setsockopt(m_socketInstance, SocketLevelHCI, HCI_SocketOption::HCI_Filter, &filter, sizeof(filter)) == 0) {
                    m_filterApplied = true;
                    success = true;
                } else {
                    Poco::Logger::get("BTHCI").error("Unable to set socket options: event mask");
                }
            } else {
                Poco::Logger::get("BTHCI").error("Unable to get socket options: event mask");
            }

            return success;
        }

        void BluetoothHCIFilter::Reset()
        {
            if (m_filterApplied == true) {
                if (setsockopt(m_socketInstance, SocketLevelHCI, HCI_SocketOption::HCI_Filter, m_filterData, m_filterDataLength) != 0) {
                    // error
                }
            }

            m_filterApplied = false;
        }
    }
}
