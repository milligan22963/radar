/*--------------------------------------
  DataPacket
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <string.h>

#include <nlohmann/json.hpp>

#include "DataPacket.h"
#include "SystemDefines.h"

namespace afm
{
    namespace data
    {
        DataPacket::DataPacket()
        {
            // add a timestamp at time of creation
            // derived can override i.e. replace later
            struct timespec timeStamp;

            memset(&timeStamp, 0, sizeof(timeStamp));

            if (clock_gettime(CLOCK_REALTIME, &timeStamp) != -1) {
                char temp[32];
                // store as seconds.nanoseconds as expressed in the timespec structure
                sprintf(temp, "%ld.%09ld", timeStamp.tv_sec, timeStamp.tv_nsec);
                SetValue(sc_timeStamp, temp);
            }
        }

        DataPacket::~DataPacket()
        {
            m_packetQueue.clear();
            m_data.clear();
        }

        void DataPacket::Clone(const std::shared_ptr<IDataPacket> pPacket)
        {
            FromJSON(pPacket->ToJSON());
        }

        void DataPacket::SetValue(const std::string &name, const std::string &value)
        {
            m_data[name] = value;
        }

        bool DataPacket::GetValue(const std::string &name, std::string &value)
        {
            return GetValue(name, value, m_data);
        }

        NameValuePairs DataPacket::GetValues()
        {
            NameValuePairs nameValues;

            for (nlohmann::json::iterator iter = m_data.begin(); iter != m_data.end(); iter++) {
                nameValues[iter.key()] = iter.value();
            }
            
            return nameValues;
        }

        bool DataPacket::RemoveValue(const std::string &name)
        {
            bool success = true;

            m_data.erase(name);

            return success;
        }

        std::string DataPacket::ToJSON()
        {
            return m_data.dump();
        }

        bool DataPacket::FromJSON(const std::string &jsonData)
        {
            bool success = true;

            try {
                m_data = nlohmann::json::parse(jsonData);
            }

            catch (const nlohmann::json::parse_error &e) {
                success = false;
            }

            return success;
        }

        bool DataPacket::FromRaw(const RawData &rawData)
        {
            return false; // no idea how handle this one
        }

        bool DataPacket::HasMultiplePackets()
        {
            return m_packetQueue.empty() == false;
        }

        IDataPacketSPtr DataPacket::getNextPacket()
        {
            IDataPacketSPtr pNextPacket = nullptr;

            if (m_packetQueue.empty() == false) {
                pNextPacket = m_packetQueue.front();
                m_packetQueue.pop_front();
            }

            return pNextPacket;
        }

        bool DataPacket::GetTag(const std::string &tagName, std::string &tagValue)
        {
            bool success = false;

            PacketTags::const_iterator iter = m_tags.find(tagName);
            if (iter != m_tags.end()) {
                tagValue = iter->second;
                success = true;
            }
            return success;
        }

        void DataPacket::SetTag(const std::string &tagName, const std::string &tagValue)
        {
            m_tags[tagName] = tagValue;
        }

        void DataPacket::Reset()
        {
            m_data.clear();
        }

        // internal
        bool DataPacket::GetValue(const std::string &name, std::string &value, nlohmann::json &dataSource)
        {
            bool success = false;

            auto iter = dataSource.find(name);

            if (iter != dataSource.end()) {
                value = iter.value();

                success = true;
            } else {
                for (auto& data : dataSource) {
                    // we have checked the top level, look deeper to find
                    // any sub values i.e. look inside of various arrays
                    // this will break on the first match...
                    if (data.is_array() == true) {
                        for (auto &item : data) {
                            // check each item in the array
                            success = GetValue(name, value, item);
                            if (success == true) {
                                break;
                            }
                        }
                    }
                }
            }

            return success;    
        }
    }
}
