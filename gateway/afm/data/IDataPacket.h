/*--------------------------------------
  IDataPacket
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_IDATAPACKET
#define _H_IDATAPACKET

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "NameValuePairs.h"

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
        // 
        /**
         * @brief raw data structure i.e. vector of bytes
         */
        using RawData = std::vector<uint8_t>;

        // predefined for SPtr declaration
        class IDataPacket;

        using IDataPacketSPtr = std::shared_ptr<IDataPacket>;

        /**
         * @class IDataPacket
         * 
         * @brief abstract implementation for Data packets
         */
        class IDataPacket
        {
            public:
                /**
                 * @brief default descructor
                 */
                virtual ~IDataPacket() {};

                /**
                 * @brief Clones a packet
                 * 
                 * @param[in] pPacket - the packet to clone
                 */
                virtual void Clone(const std::shared_ptr<IDataPacket> pPacket) = 0;

                /**
                 * @brief adds/sets a name value pair to the packet
                 * 
                 * @param[in] name - the name of the value
                 * @param[in] value - the value of the value
                 */
                virtual void SetValue(const std::string &name, const std::string &value) = 0;

                /**
                 * @brief get the value from the packet if it is valid
                 * 
                 * @param[in] name - the name of the value
                 * @param[out] value - the value of the value
                 * 
                 * @return bool - true if it exists, false otherwise
                 */
                virtual bool GetValue(const std::string &name, std::string &value) = 0;

                /**
                 * @brief get the set of values from the packet
                 * 
                 * @return a set of name value pairs for this packet
                 */
                virtual NameValuePairs GetValues() = 0;

                /**
                 * @brief removes the value from the set of values
                 * 
                 * @param[in] name - the name of the value to remove
                 * 
                 * @return bool true on success, false otherwise
                 */
                virtual bool RemoveValue(const std::string &name) = 0;

                /**
                 * @brief converts the packet to JSON string
                 * 
                 * @return std::string - the JSON representation of the packet
                 */
                virtual std::string ToJSON() = 0;

                /**
                 * @brief extracts the packet data from the JSON string
                 * 
                 * @return true on success, false otherwise
                 */
                virtual bool FromJSON(const std::string &jsonData) = 0;

                /**
                 * @brief extracts data from a raw packet data string
                 *
                 * @param[in] rawData - the rawData to consume
                 * 
                 * @return true on success, false otherwise
                 */
                virtual bool FromRaw(const RawData &rawData) = 0;

                /**
                 * @brief returns true if there are multiple packets
                 *
                 * @return true when multiple packets, false otherwise
                 */
                virtual bool HasMultiplePackets() = 0;

                /**
                 * @brief returns a tag if there is one of the
                 *      the given name.
                 * 
                 * @param[in] tagName - the name of the tag to retreive
                 * @param[out] tagValue - the value of the tag if there
                 * 
                 * @return bool - true indicating success, false indicating tag doesn't exist
                 */
                virtual bool GetTag(const std::string &tagName, std::string &tagValue) = 0;

                /**
                 * @brief sets a tag of the given name and with the value
                 * 
                 * @param[in] tagName - the name of the tag to set
                 * @param[in] tagValue - the value of the tag to set to
                 */
                virtual void SetTag(const std::string &tagName, const std::string &tagValue) = 0;

                /**
                 * @brief some packets may consist of a collection
                 *          of packets so we allow for multiples.
                 *
                 * @return the next data packet in the queue if multiples, nullptr at end
                 */
                virtual IDataPacketSPtr getNextPacket() = 0;

                /**
                 * @brief resets the packet to an empty state
                 */
                virtual void Reset() = 0;
        };

        using DataPacketDeque = std::deque<IDataPacketSPtr>;
    }
}
#endif