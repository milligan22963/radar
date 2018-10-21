/*--------------------------------------
  DataPacket
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_DATAPACKET
#define _H_DATAPACKET

#include <map>
#include <string>
#include <nlohmann/json.hpp>

#include "IDataPacket.h"
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
        /**
         * @class DataPacket
         * 
         * @brief concrete implementation for Data packets
         */
        class DataPacket : public IDataPacket
        {
            public:
                /**
                 * @brief default constructor
                 */
                DataPacket();

                /**
                 * @brief default descructor
                 */
                virtual ~DataPacket();

                /**
                 * @copydoc IDataPacket::Clone
                 */
                virtual void Clone(const std::shared_ptr<IDataPacket> pPacket) override;

                /**
                 * @copydoc IDataPacket::SetValue
                 */
                virtual void SetValue(const std::string &name, const std::string &value) override;

                /**
                 * @copydoc IDataPacket::GetValue
                 */
                virtual bool GetValue(const std::string &name, std::string &value) override;

                /**
                 * @copydoc IDataPacket::GetValues
                 */
                virtual NameValuePairs GetValues() override final;

                /**
                 * @copydoc IDataPacket::RemoveValue
                 */
                virtual bool RemoveValue(const std::string &name) override;

                /**
                 * @copydoc IDataPacket::ToJSON
                 */
                virtual std::string ToJSON() override;

                /**
                 * @copydoc IDataPacket::FromJSON
                 */
                virtual bool FromJSON(const std::string &jsonData) override;

                /**
                 * @copydoc IDataPacket::FromRaw
                 */
                virtual bool FromRaw(const RawData &rawData) override;

                /**
                 * @copydoc IDataPacket::HasMultiplePackets
                 */
                virtual bool HasMultiplePackets() override;

                /**
                 * @copydoc IDataPacket::getNextPacket
                 */
                virtual IDataPacketSPtr getNextPacket() override;

                /**
                 * @copydoc IDataPacket::Reset
                 */
                virtual void Reset() override;

            protected:
                bool GetValue(const std::string &name, std::string &value, nlohmann::json &dataSource);
                void addDataPacket(IDataPacketSPtr pPacket) { m_packetQueue.push_back(pPacket); }

            private:
                nlohmann::json m_data;

                DataPacketDeque m_packetQueue;
        };
    }
}
#endif