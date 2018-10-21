/*--------------------------------------
  SSLWebContainer
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_SSLWEBCONTAINER
#define _H_SSLWEBCONTAINER

#include <Poco/Net/HTTPSClientSession.h>
#include <deque>

#include "WebContainer.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level namespace for container library assets
     */
    namespace container
    {
        /**
         * @class SSLWebContainer
         * 
         * @brief base implementation for SSLWebContainers
         */
        class SSLWebContainer : public WebContainer
        {
            public:
                /**
                 * @brief default constructor
                 */
                SSLWebContainer();

                /**
                 * @brief default descructor
                 */
                virtual ~SSLWebContainer();

            protected:
                /**
                 * @copydoc WebContainer::CreateClientSession
                 */
                virtual HTTPClientSessionSPtr CreateClientSession(const Poco::URI &uri);

            private:
                Poco::Net::Context::Ptr m_pContext = nullptr;
        };
    }
}
#endif