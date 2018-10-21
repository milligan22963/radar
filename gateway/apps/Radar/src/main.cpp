/*--------------------------------------
  Radar
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <memory>

#include "Radar.h"

int main(int argc, char **argv)
{
  std::shared_ptr<afm::application::Radar> pApp = std::make_shared<afm::application::Radar>();
  try
  {
    pApp->run(argc, argv);
  }
  catch (Poco::Exception& exc)
  {
    pApp->logger().log(exc);
    return Poco::Util::Application::EXIT_CONFIG;
  }
  return 0;
}