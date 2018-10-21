/*--------------------------------------
  SSLWebContainer
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include "SSLWebContainer.h"

namespace afm
{
    namespace container
    {
        class SSLInit
        {
            public:
                SSLInit()
                {
                    Poco::Net::initializeSSL();
                }

                virtual ~SSLInit()
                {
                    Poco::Net::uninitializeSSL();
                }

        };

        SSLInit g_SSLInitializer;

        SSLWebContainer::SSLWebContainer()
        {

        }

        SSLWebContainer::~SSLWebContainer()
        {
            m_pContext = nullptr;
        }

        HTTPClientSessionSPtr SSLWebContainer::CreateClientSession(const Poco::URI &uri)
        {
            // context is a poco auto ptr, will get cleaned
            m_pContext = new Poco::Net::Context(
                Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem",
                Poco::Net::Context::VERIFY_STRICT, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
 
             HTTPClientSessionSPtr pClientSession = std::make_shared<Poco::Net::HTTPSClientSession>(
                 uri.getHost(),
                 uri.getPort(),
                 m_pContext);

            return pClientSession;
        }
    }
}