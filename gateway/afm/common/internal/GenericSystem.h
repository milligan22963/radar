/*--------------------------------------
  GenericSystem

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_GENERIC_SYSTEM
#define _H_GENERIC_SYSTEM

#include "ISystem.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level namespace for common library assets
     */
    namespace common
    {
        /**
         * @class GenericSystem
         * 
         * @brief base implementation for systems
         */
        class GenericSystem : public ISystem
        {
            public:
                /**
                 * @brief default constructor
                 */
                GenericSystem();

                /**
                 * @brief default descructor
                 */
                virtual ~GenericSystem();

                virtual bool Initialize() override;

                virtual std::string GetSerialNumber() const override;

            protected:
                bool ProcessSerialNumberFile(const std::string &fileName);

            private:
                std::string m_serialNumber = "none";
        };
    }
}
#endif