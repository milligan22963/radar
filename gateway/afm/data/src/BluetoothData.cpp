/*--------------------------------------
  BluetoothPacket
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <sstream>
#include <Poco/Logger.h>
#include "common/BluetoothHCI.h"
#include "BluetoothData.h"

namespace afm
{
    namespace data
    {
		static const std::string sc_bdAddress = "address";
		static const std::string sc_addressType = "addressType";
		static const std::string sc_temperature = "temperature";
		static const std::string sc_humidity = "humidity";
		static const std::string sc_rssi = "rssi";
	//4c00 02 15 00 05 00 01 00 00 10 00 80 00 00 80 5f 9b 01 31 00156b67c3a0
		static const std::vector<uint8_t> sc_cypressUUID = {
			0x00, 0x05, 0x00, 0x01, 0x00, 0x00,
			0x10, 0x00, 0x80, 0x00, 0x00,
			0x80, 0x5f, 0x9b, 0x01, 0x31};

		static const uint16_t sm_maxAdvertisingReports = 25;
		static const uint8_t sm_maxAdvertisingDataLength = 0x1F;

		/**
		 * Types of LE addresses
		 */
		 enum LE_ADDRESS_TYPE
		 {
			 LE_PUBLIC_ADDRESS = 0x00,
			 LE_RANDOM_ADDRESS,
			 END_LE_ADDRESS_TYPES
		 };
 
		const int sc_minimumPacketSize = 7; // at least seven bytes with ff before the manufacturer id

		/**
		 * Report type
		 */
		const uint8_t sc_reportType = 3; // ADV NON CONNECTABLE
        
        BluetoothDataPacket::BluetoothDataPacket()
        {

        }

        BluetoothDataPacket::~BluetoothDataPacket()
        {
        }

        bool BluetoothDataPacket::FromRaw(const RawData &rawData)
        {
            bool success = false;

			LEAdvertisingPacket *pAdvertisingPacket = (LEAdvertisingPacket *)rawData.data();
			if (pAdvertisingPacket->packetType == common::HCI_PacketType::HCI_EventPacket) {
				if (pAdvertisingPacket->eventType == common::HCI_EventType::HCI_LEMetaEvent) {
					LE_ADVERTISING_DATA_PACKET *pAdvertisingData = (LE_ADVERTISING_DATA_PACKET *)&pAdvertisingPacket->pPacketData;

					if (pAdvertisingData->eventType == common::LE_MetaSubEventCode::LE_AdvertisingReport) {
						// Process each of the advertising reports up to 25
						for (uint8_t report = 0; report < pAdvertisingData->numberReports; report++) {
							// we have to have at least one valid report to make it valid
							BluetoothDataPacketSPtr pPacket = ProcessReport(&pAdvertisingData->reports[report]);

							if (pPacket != nullptr) {
								pPacket->SetValue("rawData", ConvertRawToString(rawData));
								addDataPacket(pPacket);
								success = true;
							}
						}
					}
				}
			}

            return success;
        }

		std::string BluetoothDataPacket::ConvertRawToString(const RawData &rawData)
		{
			char temp[16];			
			std::string rawConversion;

			for (std::size_t index = 0; index < rawData.size(); index++) {
				snprintf(temp, 16, "%02x", rawData[index]);
				rawConversion.append(temp);
			}
			return rawConversion;
		}

		std::string BluetoothDataPacket::ConvertBDAddress(BLUETOOTH_ADDRESS *pAddress)
		{
			std::string address;
			char temp[16];
			uint8_t *pPtr = (uint8_t *)pAddress;

			// bd addrs are lsB first
			for (int index = 0; index < sm_bdAddressLength; index++) {
				snprintf(temp, 16, "%02x", pPtr[index]);
				address.insert(0, temp);
			}

			return address;
		}

		BluetoothDataPacketSPtr BluetoothDataPacket::ProcessReport(LE_ADVERTISING_DATA_REPORT *pReport)
		{
			BluetoothDataPacketSPtr pPacket = nullptr;

			// right now we are looking for basic non-connectable advertisements i.e.
			// beacons ala fruit company
			// https://developer.apple.com/ibeacon/
			if (pReport->reportType == sc_reportType) {
				LE_ReportData *pReportData = (LE_ReportData *)pReport->payload;
				// manufacturer specific
				if (pReportData->manufacturerType == sm_manufacturerSpecific) {

					if ((pReportData->companyId[0] == 0x4c) && (pReportData->companyId[1] == 0x00)) {
						LEBeaconData *pBeaconData = (LEBeaconData *)&pReportData->beaconData;

						// cypress?
						if (std::equal(sc_cypressUUID.begin(), sc_cypressUUID.end(), pBeaconData->uuid) == true) {
							char temp[16];

							pPacket = std::make_shared<BluetoothDataPacket>();

							pPacket->SetValue(sc_bdAddress, ConvertBDAddress(&pReport->address));

							snprintf(temp, 16, "%d", pReport->getRSSI());
							pPacket->SetValue(sc_rssi, temp);

							pPacket->SetValue(sc_humidity, ConvertCypressHumitidy(pBeaconData->opt.data[0]));
							pPacket->SetValue(sc_temperature, ConvertCypressTemperature(pBeaconData->opt.data[1]));
						}
					}
				}
			}
			return pPacket;
		}

		std::string BluetoothDataPacket::ConvertCypressTemperature(uint8_t value)
		{
			// based on cypress data sheet
			float convertedData = ((175.72f * (value * 256)) / 65536) - 46.85f; // convert to celcius

			std::stringstream temperature;

			temperature << (convertedData + 273.0f); // convert to Kelvin

			return temperature.str();
		}

		std::string BluetoothDataPacket::ConvertCypressHumitidy(uint8_t value)
		{
			std::stringstream humidity;

			// based on cypress data sheet
			float convertedData = ((125.0f * (value * 256)) / 65536) - 6.0f; // convert to %

			humidity << convertedData;

			return humidity.str();
		}
    }
}
