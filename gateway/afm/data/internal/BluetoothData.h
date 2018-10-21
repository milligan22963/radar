/*--------------------------------------
  BluetoothPacket
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_BLUETOOTHPACKET
#define _H_BLUETOOTHPACKET

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "DataPacket.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level namespace for data library assets
     */
    namespace data
    {
        static const uint8_t sm_manufacturerSpecific = 0xFF;

        // example data over the air
        // things typically are length type value
        // "rawData":"
        // 043e2a020103001c2a0750a0001e0201041aff4c0002150005000100001000800000805f9b013100107167c3a1"
        // 04 = b00000100 (HCI_EventPacket)
        // 3e = event code (HCI_LEMetaEvent)
        // 2a = length of 42

        struct __attribute__((packed)) LEAdvertisingPacket
        {
            uint8_t packetType;
            uint8_t eventType;
            uint8_t packetLength;
            void    *pPacketData;
        };

        // after length we will have all sorts of goodness
        // 02 01 03 00 1c2a0750a000 1e0201041aff4c0002150005000100001000800000805f9b013100107167c3a1
        // 02 - report type
        // 01 - number of reports

        // 03 report #1 type
        // 00 address type
        // address 1c2a0750a00 (lsB to msB)

		/**
		 * The structure defining a bluetooth address
		 * which is the same as a mac address i.e. 6 uint_8's
		 */
        static const int sm_bdAddressLength = 6;

		struct __attribute__((packed)) BLUETOOTH_ADDRESS
		{
			uint8_t address[sm_bdAddressLength];
		};

		/**
		 * Structure defining an LE advertising report as received as an event
		 */
		struct __attribute__((packed)) LE_ADVERTISING_DATA_REPORT
		{
			uint8_t reportType;	// type for this report.
			uint8_t addressType;
			BLUETOOTH_ADDRESS address;
			uint8_t payloadLength; // length of data payload (not including RSSI)
			uint8_t payload[0];

			// RSSI is takced on the end of the packet one beyond the payload length
			int8_t getRSSI()
			{
				return payload[payloadLength]; // last byte is RSSI
			}
		};

        // 1e 02 01 04 1a ff 4c00 02150005000100001000800000805f9b013100107167c3a1
        // 1e - payload length - in above structure
        // 02 - flags
        // 01 - flags 1
        // 04 - flags 2
        // 1a - length
        // ff - manufacturer specific type
        // 4c00 - company id
        struct __attribute__((packed)) LE_ReportData
        {
            uint8_t flags[3]; // 3 flags
            uint8_t length;
            uint8_t manufacturerType;
            uint8_t companyId[2];
            uint8_t beaconData[0];
        };

        // 02 15 0005000100001000800000805f9b0131 0010 7167 c3 a1
        // 02 - beacon type 0 must be 2
        // 15 - beacon type 1 must be 15
        // 0005000100001000800000805f9b0131 - uuid
        // 0010 major id 
        // 7167 minor id
        // c3 power setting at 1M from a fruit
        // a1 - rssi

		/**
		 * Structure defining an LE advertising report as received as an event
		 */
		struct __attribute__((packed)) LE_ADVERTISING_DATA_PACKET
		{
			uint8_t eventType;
			uint8_t numberReports; // reports can be from 1 to 25
			LE_ADVERTISING_DATA_REPORT reports[0];
		};

		/**
		 * Structure defining the advertising stucture header
		 */
		struct __attribute__((packed)) LE_ADVERTISING_DATA_STRUCTURE_HEADER
		{
			uint8_t length; // length of the combined type and data fields
			uint8_t type;
		};

		/**
		 * Structure defining an advertising data structure
		 */
		struct __attribute__((packed)) LE_ADVERTISING_DATA_STRUCTURE : public LE_ADVERTISING_DATA_STRUCTURE_HEADER
		{
			uint8_t data[0];
		};

		/**
		* Structure defining a manufacturing-specific advertising structure
		*/
		struct __attribute__((packed)) LE_ADVERTISING_DATA_MANUFACTURE_SPECIFIC : public LE_ADVERTISING_DATA_STRUCTURE_HEADER
		{
			uint8_t companyId[2];
			uint8_t data[0];
		};

		/*
		* Apple beacon format
		*
		* From apples guide on beacons
		*
		* This structure contains only those included in the manufacturing-specific data
		*/
		struct __attribute__((packed)) LEBeaconData
		{
			uint8_t 	beaconType[2];
			uint8_t 	uuid[16];
			uint16_t 	major;
			union opt_t
			{
				uint16_t 	minor; 		// apple
				uint8_t		data[2];	// 2 data bytes (Cypress humidity = data[0], temperature = data[1]
			} opt;
			int8_t		power;
		};

        // pre-declaration for shared ptr definition
        class BluetoothDataPacket;

        using BluetoothDataPacketSPtr = std::shared_ptr<BluetoothDataPacket>;

        /**
         * @class BluetoothDataPacket
         * 
         * @brief concrete implementation for Data packets
         */
        class BluetoothDataPacket : public DataPacket
        {
            public:
                /**
                 * @brief default constructor
                 */
                BluetoothDataPacket();

                /**
                 * @brief default descructor
                 */
                virtual ~BluetoothDataPacket();

                /**
                 * @copydoc IDataPacket::FromRaw
                 */
                virtual bool FromRaw(const RawData &rawData) override;

            private:
                std::string ConvertRawToString(const RawData &rawData);
                std::string ConvertBDAddress(BLUETOOTH_ADDRESS *pAddress);
                BluetoothDataPacketSPtr ProcessReport(LE_ADVERTISING_DATA_REPORT *pReport);
                std::string ConvertCypressTemperature(uint8_t value);
                std::string ConvertCypressHumitidy(uint8_t value);
        };
    }
}
#endif