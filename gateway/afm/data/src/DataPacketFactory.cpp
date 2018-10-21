/*--------------------------------------
  DataPacketFactory
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include "DataPacketFactory.h"
#include "DataPacket.h"
#include "BluetoothData.h"

namespace afm
{
    namespace data
    {
        IDataPacketSPtr DataPacketFactory::CreateDataPacket(DataPacketType type)
        {
            IDataPacketSPtr pPacket = nullptr;

            switch (type) {
                case DataPacketType::DATA: {
                    pPacket = std::make_shared<DataPacket>();
                }
                break;
                case DataPacketType::BLUETOOTH: {
                    pPacket = std::make_shared<BluetoothDataPacket>();
                }
                break;
                default: {

                }
                break;
            }
            return pPacket;
        }

        DataPacketFactory::DataPacketFactory()
        {

        }

        DataPacketFactory::~DataPacketFactory()
        {

        }
    }
}