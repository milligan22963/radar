/*--------------------------------------
  ISystem

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_ISYSTEM
#define _H_ISYSTEM

#include <string>
#include <memory>

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
         * @interface ISystem
         * 
         * @brief base implementation for systems
         */
        class ISystem
        {
            public:
                /**
                 * @brief default descructor
                 */
                virtual ~ISystem() {}

                virtual bool Initialize() = 0;
                
                virtual std::string GetSerialNumber() const = 0;
        };

        using ISystemSPtr = std::shared_ptr<ISystem>;
    }
}
#endif