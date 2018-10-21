/*--------------------------------------
  GenericSystem

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <fstream>
#include <string.h>
#include "GenericSystem.h"

namespace afm
{
    namespace common
    {
        static const std::string sc_serialNumberPath = "/sys/class/dmi/id/product_serial";
        static const std::string sc_secondarySerialNumberPath = "/sys/class/dmi/id/board_serial";

        GenericSystem::GenericSystem()
        {

        }

        GenericSystem::~GenericSystem()
        {

        }

        bool GenericSystem::Initialize()
        {
            bool success = false;
            
            success = ProcessSerialNumberFile(sc_serialNumberPath);
            if (success == false) {
                success = ProcessSerialNumberFile(sc_secondarySerialNumberPath);
            }
            return success;
        }

        std::string GenericSystem::GetSerialNumber() const
        {
            return m_serialNumber;
        }

        bool GenericSystem::ProcessSerialNumberFile(const std::string &fileName)
        {
            bool success = false;
            std::ifstream serialNumberFile;

            serialNumberFile.open(fileName, std::ios_base::in);

            if (serialNumberFile.is_open() == true) {
                char serialNumber[128];

                serialNumberFile.getline(serialNumber, 128);
                if (strlen(serialNumber) > 0) {
                    m_serialNumber = serialNumber;
                    success = true;
                }
                serialNumberFile.close();
            }
            return success;
        }
    }
}
