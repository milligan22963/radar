/*--------------------------------------
  SystemFactory

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_SYSTEM_FACTORY
#define _H_SYSTEM_FACTORY

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
         * @class SystemFactory
         * 
         * @brief for creating systems based on the running system
         */
        class SystemFactory
        {
            public:
                static ISystemSPtr GetSystem();
        };
    }
}
#endif