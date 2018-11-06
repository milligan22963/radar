/*--------------------------------------
  Radar

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>
#include <unistd.h>
#include "SystemDefines.h"
#include "Radar.h"
#include "data/DataPacketFactory.h"

namespace afm
{
    namespace application
    {
        // defined in Radar.cfg so must match what is there
        // once moved to DB it will come from there
        static const std::string sc_bluetoothContainer = "bt";
        static const std::string sc_mqttContainer = "mqtt";

        Radar::Radar()
            : AFMApp("Radar")
        {

        }

        void Radar::OnPacketWritten(const std::string &containerId, const data::IDataPacketSPtr pPacket, bool success)
        {

        }

        void Radar::OnPacketRead(const std::string &containerId, data::IDataPacketSPtr pPacket, bool success)
        {
            if (success == true) {
                if (containerId == sc_bluetoothContainer) {
                    pPacket->RemoveValue(container::sc_id);
                    std::string address;
                    pPacket->GetValue("address", address);

                    pPacket->SetTag(sc_topic, "afm/sensor/" + address);

                    // We send directly to the offload interface as we
                    // can utilize its persistence
                    if (IsShuttingDown() == false) {
                        if (m_offload != nullptr) {
                            m_offload->WritePacket(pPacket);
                        }
                    }
                } else if (containerId == sc_mqttContainer) {
                    // configuration information
                }
            }
            GetLogger().information("Read: %s", pPacket->ToJSON());
        }

        void Radar::OnContainerReady(const std::string &containerId, bool isReady)
        {
            if (isReady == true) {
                GetLogger().information("Container (%s) is ready.", containerId);
            } else {
                GetLogger().information("Container (%s) is not ready.", containerId);
            }
        }

        /**
         * Protected portions
         */
        void Radar::OnStartup()
        {
            common::ISettingsSPtr pSettings = GetSettings();

            if (pSettings != nullptr) {
                m_receiver = pSettings->GetContainer(sc_bluetoothContainer);
                m_offload = pSettings->GetContainer(sc_mqttContainer);
            }
        }

        void Radar::OnShutdown()
        {
            m_receiver = nullptr;
            m_offload = nullptr;
        }
    }
}