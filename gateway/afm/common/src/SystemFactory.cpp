/*--------------------------------------
  SystemFactory

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include "SystemFactory.h"
#include "GenericSystem.h"

namespace afm
{
    namespace common
    {
        ISystemSPtr SystemFactory::GetSystem()
        {
            static ISystemSPtr pSystem = nullptr;

            if (pSystem == nullptr) {
                pSystem = std::make_shared<GenericSystem>();

                pSystem->Initialize();
            }

            return pSystem;
        }
    }
}
