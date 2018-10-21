/*--------------------------------------
  File

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <Poco/Util/Application.h>
#include <Poco/Logger.h>

#include "container/ContainerFactory.h"

namespace afm
{
    namespace application
    {
        class Mqtt : public Poco::Util::Application
        {
            public:
                Mqtt();
                virtual ~Mqtt();
                
            protected:
                /**
                 * @copydoc Poco::Util::Application::initialize
                 */
                virtual void initialize(Application &self) override;

                /**
                 * @copydoc Poco::Util::Application::main
                 */
                virtual int main(const std::vector<std::string> &args) override;

            private:
                container::IContainerSPtr m_pContainer = nullptr;
                Poco::Logger &m_logger;
        };
    }
}