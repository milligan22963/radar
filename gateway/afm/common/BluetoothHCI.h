/*--------------------------------------
  BluetoothHCI
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_BLUETOOTHHCI
#define _H_BLUETOOTHHCI

#include <cstdint>
#include <sys/socket.h>

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level for common definitions
     * 
     * @note specifications found at https://www.bluetooth.com/specifications/bluetooth-core-specification
     */
    namespace common
    {
        /**
         * @brief enumerations and related from BT Spec, right now only
         *          defining the minimum amount to work with
         * 
         * based on 5.0
         * 
         * Section Volume 2
         */

        /**
         * @brief command types
         * 
         * Section Volume 2, Part E, Section 4
         * 
         * See also: https://iotbreaks.com/understand-bluetooth-hci-commands-and-events/
         */
        enum HCI_PacketType : uint8_t {
            HCI_CommandPacket = 0x01,
            HCI_ACLPacket,
            HCI_SCOPacket,
            HCI_EventPacket,
            END_HCI_PACKET_TYPES
        };

        /**
         * The structure of an HCI command being sent over
         * a socket
         */
        struct __attribute__((packed)) HCI_Command {
            uint16_t        commandCode;
            uint8_t        parameterLength;
            void        *pParameters;
        };

        // define sie so we don't include the parameter pointer
        static const uint8_t HCI_CommandSize = 3; // doesnt' include void *

        enum HCI_CommandFields {
            HCI_PacketTypeField,
            HCI_CommandStructure,
            HCI_ParameterFields,
            NUM_HCI_CommandFields
        };


        static const int32_t SocketLevelHCI = 0;

        enum HCI_SocketOption {
            HCI_DataDirection = 1,
            HCI_Filter,
            HCI_TimeStamp,
            HCI_EventFilter,
            END_HCI_SOCKET_OPTIONS
        };

        enum Bluetooth_ProtocolLayer {
            BT_L2CAP = 0,
            BT_HCI,
            BT_SCO,
            BT_RFCOMM,
            BT_BNEP,
            BT_CMTP,
            BT_HIDP,
            BT_AVDTP,
            END_BLUETOOTH_PROTOCOL_LAYERS
        };

        /**
         * The HCI socket address format
         */
        struct HCI_SockAddr {
            sa_family_t hci_family;
            uint16_t 	hci_device;
            uint16_t 	hci_channel;
        };

        /**
         * @brief Event Types
         * 
         * Section Volume 2, Part E, Section 7.7 +
         */
        enum HCI_EventType : uint8_t {
            HCI_InquiryComplete = 0x01,
            HCI_InquiryResult,
            HCI_ConnectionComplete,
            HCI_ConnectionRequest,
            HCI_DisconnectComplete,
            HCI_AuthenticationComplete,
            HCI_RemoteNameRequestComplete,
            HCI_EncryptionChange,
            HCI_ChangeConnectionLinkKeyComplete,
            HCI_MasterLinkKeyComplete,
            HCI_ReadRemoteSupportedFeaturesComplete,
            HCI_ReadRemoteVersionInformationComplete,
            HCI_QOSSetupComplete,
            HCI_CommandComplete,
            HCI_CommandStatus,
            HCI_HardwareError,
            HCI_FlushOccurred,
            HCI_RoleChange,
            HCI_NumberOfCompletedPackets,
            HCI_ModeChange,
            HCI_ReturnLinkKeys,
            HCI_PinCodeRequest,
            HCI_LinkKeyRequest,
            HCI_LinkKeyNotification,
            HCI_LoopbackCommand,
            HCI_DataBufferOverflow,
            HCI_MaxSlotsChange,
            HCI_ReadClockOffsetComplete,
            HCI_ConnectionPacketTypeChanged,
            HCI_QOSViolation,
            HCI_PageScanRepetitionModeChange = 0x20,
            HCI_FlowSpecificationComplete,
            HCI_InquiryResultWithRSSI,
            HCI_ReadRemoteExtendedFeaturesComplete,
            HCI_SynchronousConnectionComplete = 0x2C,
            HCI_SynchronousConnectionChanged,
            HCI_SniffSubrating,
            HCI_ExtendedInquiryResult,
            HCI_EncryptionKeyRefreshComplete,
            HCI_IOCapabilityRequest,
            HCI_IOCapabilityResponse,
            HCI_UserConfirmationRequest,
            HCI_UserPasskeyRequest,
            HCI_RemoteOOBDataRequest,
            HCI_SimplePairingComplete,
            HCI_LinkSupervisionTimeoutChanged = 0x38,
            HCI_EnhancedFlushComplete,
            HCI_UserPasskeyNotification = 0x3B,
            HCI_KeypressNotification,
            HCI_RemoteHostSupportedFeaturesNotification,
            HCI_PhysicalLinkComplete = 0x40,
            HCI_ChannelSelected,
            HCI_DisconnectionPhysicalLinkComplete,
            HCI_PhysicalLinkLossEarlyWarning,
            HCI_PhysicalLinkRecovery,
            HCI_LogicalLinkComplete,
            HCI_DisconnectionLogicalLinkComplete,
            HCI_FlowSpecModifyComplete,
            HCI_NumberOfCompletedDataBlocks,
            HCI_ShortRangeModeChangeComplete = 0x4C,
            HCI_AmpStatusChange,
            HCI_AmpStartTest = 0x49,
            HCI_AmpTestEnd,
            HCI_AmpReceiverReport,
            HCI_LEMetaEvent = 0x3E,
            HCI_TriggeredClockCapture = 0x4E,
            HCI_SynchronizationTrainComplete,
            HCI_SynchronizationTrainReceived,
            HCI_ConnectionlessSlaveBroadcast,
            HCI_ConnectionlessSlaveBroadcastTimeout,
            HCI_TruncatedPageComplete,
            HCI_SlavePageResponseTimeout,
            HCI_ConnectionlessSlaveBroadcastChannelMapChange,
            HCI_InquiryResponseNotification,
            HCI_AuthenticatedPayloadTimeoutExpired,
            HCI_SamStatusChange,
            END_HCI_EventTypes
        };

        /**
         * @brief LE Meta event sub codes
         * 
         * Section Volume 2, Part E, Section 7.7.65 +
         */
        enum LE_MetaSubEventCode {
            LE_ConnectionComplete = 0x01,
            LE_AdvertisingReport,
            LE_ConnectionUpdateComplete,
            LE_ReadRemoteFeaturesComplete,
            LE_LongTermKeyRequest,          // LTK
            LE_RemoteConnectionParameterRequest,
            LE_DataLengthChange,
            LE_ReadLocalP256PublicKey,
            LE_GenerateDHKeyComplete,
            LE_EnhancedConnectionComplete,
            LE_DirectedAdvertisingReport,
            LE_PhyUpdateComplete,
            LE_ExtendedAdvertisingReport,
            LE_PeriodicAdvertisingSyncEstablished,
            LE_PeriodicAdvertisingReport,
            LE_PeriodicAdvertisingSyncLost,
            LE_ScanTimeout,
            LE_AdvertisingSetTerminated,
            LE_ScanRequestReceived,
            LE_ChannelSelectionAlgorithm,
            END_LE_META_SUB_EVENT_CODE
        };

        struct __attribute__((packed)) HCI_Event {
            uint8_t eventCode;
            uint8_t parameterLength;
            void 	*pParams;
        };

        // define size so we don't include the void *
        static const uint8_t HCI_EventSize = 2; // without params void *

        struct __attribute__((packed)) HCI_FilterParams {
            uint32_t packetTypeMask;
            uint32_t eventMask[2];
            uint16_t controllerCommand;
        };

        /**
         * The structure for setting LE scan parameters over the HCI socket
         */
        struct __attribute__((packed)) LE_ScanParams {
            uint8_t        type;
            uint16_t       interval;
            uint16_t       window;
            uint8_t        localAddressType;
            uint8_t        filterDuplicates;
        };

        /**
         * The structure for enabling LE scanning
         */
        struct __attribute__((packed)) LE_ScanEnableParams {
            uint8_t enable;
            uint8_t filterDuplicates;
        };

        #define OGF_OCF(x, y) ((x << 10) | y)

        /**
         * @brief LE Controller Commands
         * 
         * Section Volume 2, Part E, Section 7.8 +
         */
        enum LE_ControllerCommand : uint16_t {
            LE_SetEventMask                     = OGF_OCF(0x08, 0x01),
            LE_ReadBufferSize                   = OGF_OCF(0x08, 0x02),
            LE_ReadLocalSupportedFeatures       = OGF_OCF(0x08, 0x03),
            LE_SetRandomAddress                 = OGF_OCF(0x08, 0x05),
            LE_SetAdvertisingParameters         = OGF_OCF(0x08, 0x06),
            LE_ReadAdvertisingChannelTxPower    = OGF_OCF(0x08, 0x06),
            LE_SetAdvertisingData               = OGF_OCF(0x08, 0x08),
            LE_SetScanResponseData              = OGF_OCF(0x08, 0x09),
            LE_SetAdvertisingEnable             = OGF_OCF(0x08, 0x0A),
            LE_SetScanParameters                = OGF_OCF(0x08, 0x0B),
            LE_SetScanEnable                    = OGF_OCF(0x08, 0x0C),
            END_LE_CONTROLLER_COMMANDS
        };

        /**
         * @brief LE Event Mask options
         */
        enum LE_EventMask {
            LE_MASK_ConnectionComplete = 0,
            LE_MASK_AdvertisingReport,
            LE_MASK_ConnectionUpdateComplete,
            LE_MASK_ReadRemoteFeaturesComplete,
            LE_MASK_LongTermKeyRequest,
            LE_MASK_RemoteConnectionParameterRequest,
            LE_MASK_DataLengthChange,
            LE_MASK_ReadLocalP256PublicKey,
            LE_MASK_GenerateDHKeyComplete,
            LE_MASK_EnhancedConnectionComplete,
            LE_MASK_DirectedAdvertisingReport,
            LE_MASK_PhyUpdateComplete,
            LE_MASK_ExtendedAdvertisingReport,
            LE_MASK_PeriodicAdvertisingSyncEstablished,
            LE_MASK_PeriodicAdvertisingReport,
            LE_MASK_PeriodicAdvertisingSyncLost,
            LE_MASK_ExtendedScanTimeout,
            LE_MASK_ExtendedAdvertisingSetTerminated,
            LE_MASK_ScanRequestReceived,
            LE_MASK_ChannelSelectionAlgorithm,
            END_LE_EVENT_MASKS
        };

        /**
         * @class BluetoothHCIFilter
         * 
         * @brief a class for managing event filters
         */
        class BluetoothHCIFilter
        {
            public:
                /**
                 * @brief standard constructor
                 */
                BluetoothHCIFilter();

                /**
                 * @brief standard destructor
                 */
                virtual ~BluetoothHCIFilter();

                /**
                 * @brief sets the socket this filter is being used with
                 *          this cannot be changed once applied
                 *
                 * @param[in] socketInstance - the socket instance/handle to use
                 */
                void AssociateSocket(int32_t socketInstance);

                /**
                 * @brief sets the packet type the filter is for
                 *
                 * @param[in] packetType - the type of packet this applies to
                 */
                void SetPacketType(HCI_PacketType packetType)
                {
                    SetBit(packetType, &m_packetTypeMask);
                }

                /**
                 * @brief sets controller command to be issued
                 *
                 * @param[in] command - the command to be issued
                 */
                void SetControllerCommand(LE_ControllerCommand command) { m_controllerCommand = command; }

                /**
                 * @brief enables the mask for the given event
                 *
                 * @param[in] maskBit - the bit to mask
                 */
                void EnableMask(HCI_EventType maskBit)
                {
                    setBit(maskBit & sm_maxEventBits, &m_eventMask2);
                }

                /**
                 * @brief disables the mask for the given event
                 *
                 * @param[in] maskBit - the bit to mask
                 */
                void DisableMask(HCI_EventType maskBit)
                {
                    ClearBit(maskBit, &m_eventMask);
                }

                /**
                 * @brief applies the given event mask to the hci controller
                 * 
                 * @return true on success, false otherwise
                 */
                bool ApplyEventMask();

                /**
                 * @brief resets the filter back to what it was
                 */
                void Reset();

            private:

                /**
                 * static/const maximum bits for 32
                 */
                const uint8_t sm_maxBits = 31; // 0 - 31 for 32 bits

                /**
                 * const max bits for a packet type
                 */
                const uint8_t sm_maxTypeBits = 31;

                /**
                 * const max bits for an event type
                 */
                const uint8_t sm_maxEventBits = 63; // 0 - 63 for 64 bits

                inline void setBit(uint8_t bitIndex, void *pTarget)
                {
                    *((uint32_t *)pTarget + (bitIndex >> 5)) |= (1 << (bitIndex & sm_maxBits));
                }

                /**
                 * @brief sets a given bit in the target field
                 *
                 * @param[in] bitIndex - the bit to set
                 * @param[in/out] pTarget - the target address to have its bit(s) set.
                 */
                inline void SetBit(uint8_t bitIndex, uint64_t *pTarget)
                {
                    *pTarget |= (1 << bitIndex);
                }

                /**
                 * @brief sets a given bit in the target field
                 *
                 * @param[in] bitIndex - the bit to set
                 * @param[in/out] pTarget - the target address to have its bit(s) set.
                 */
                inline void SetBit(uint8_t bitIndex, uint32_t *pTarget)
                {
                    *pTarget |= (1 << bitIndex);
                }

                /**
                 * @brief clears a given bit in the target field
                 *
                 * @param[in] bitIndex - the bit to clear
                 * @param[in/out] pTarget - the target address to have its bit(s) cleared.
                 */
                inline void ClearBit(uint8_t bitIndex, uint64_t *pTarget)
                {
                    *pTarget &= ~(1 << bitIndex);
                }

                /**
                 * @brief clears a given bit in the target field
                 *
                 * @param[in] bitIndex - the bit to clear
                 * @param[in/out] pTarget - the target address to have its bit(s) cleared.
                 */
                inline void ClearBit(uint8_t bitIndex, uint32_t *pTarget)
                {
                    *pTarget &= ~(1 << bitIndex);
                }

            private:
                /**
                 * True if the filter has been applied,
                 * false otherwise
                 */
                bool m_filterApplied = false;

                /**
                 * The instance of the socket this this filter is applied to
                 */
                int32_t m_socketInstance = 0;

                /**
                 * The command to set
                 */
                LE_ControllerCommand m_controllerCommand = LE_ControllerCommand::END_LE_CONTROLLER_COMMANDS;

                /**
                 * All 8 octets of event maskiness
                 */
                uint64_t m_eventMask = 0;
                uint32_t m_eventMask2[2] = {0};

                /**
                 * The current filter data as read from the HCI socket
                 */
                char m_filterData[256] = {0};

                /**
                 * The length of the filter data as read from the HCI socket
                 */
                uint32_t m_filterDataLength = 0;

                uint32_t m_packetTypeMask = 0;
        };
    }
}

#endif
