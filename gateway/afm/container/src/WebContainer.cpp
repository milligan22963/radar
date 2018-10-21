/*--------------------------------------
  WebContainer
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <sstream>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTMLForm.h>

#include <Poco/Exception.h>

#include "data/DataPacketFactory.h"
#include "WebContainer.h"

namespace afm
{
    namespace container
    {
        WebContainer::WebContainer()
        {

        }

        WebContainer::~WebContainer()
        {
            m_pClientSession = nullptr;
        }

        bool WebContainer::Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient)
        {
            bool success = Container::Initialize(options, pNotificationRecipient);

            if (success == true) {
                Poco::URI uri(GetTarget());

                m_targetPath = uri.getPathAndQuery();
                if (m_targetPath.empty()) {
                    m_targetPath = "/";
                }

                m_pClientSession = CreateClientSession(uri);

                if (m_pClientSession == nullptr) {
                    success = false;
                }

                SetIsReady(success);
            }

            return success;
        }

        bool WebContainer::WritePacket(const data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            m_responsePacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::DATA);

            m_responsePacket->Clone(pPacket);

            const ContainerOptions &options = GetOptions();

            for (auto option = options.begin(); option != options.end(); option++) {
                if (option->is_array() == false) {
                    m_responsePacket->SetValue(option.key(), option.value());
                }
            }

            Poco::Net::HTTPRequest webRequest(Poco::Net::HTTPRequest::HTTP_POST,
                m_targetPath, Poco::Net::HTTPMessage::HTTP_1_1);

            std::string requestData = m_responsePacket->ToJSON();

            webRequest.setContentType("application/json");
            webRequest.setContentLength(requestData.length());

            std::ostream &requestChannel = m_pClientSession->sendRequest(webRequest);

            requestChannel << requestData;

            Poco::Net::HTTPResponse webResponse;
            
            std::istream &responseChannel = m_pClientSession->receiveResponse(webResponse);

            if (webResponse.getStatus() == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
                std::stringstream packetData;

                packetData << responseChannel.rdbuf();

                m_responsePacket->FromJSON(packetData.str());

                success = true;
            }

            m_pClientSession->reset();

            OnPacketWritten(pPacket, success);

            return success;
        }

        bool WebContainer::ReadPacket(data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            if (m_responsePacket != nullptr) {
                pPacket->Clone(m_responsePacket);
                m_responsePacket = nullptr;
                success = true;
            } else {
                Poco::Net::HTMLForm requestForm;

                // add standard form data
                const ContainerOptions &options = GetOptions();

                for (auto option = options.begin(); option != options.end(); option++) {
                    if (option->is_array() == false) {
                        requestForm.add(option.key(), option.value());
                    }
                }

                // add packet specific data if any
                for (auto data : pPacket->GetValues()) {
                    requestForm.add(data.first, data.second);
                }

                Poco::Net::HTTPRequest webRequest(Poco::Net::HTTPRequest::HTTP_GET,
                    m_targetPath, Poco::Net::HTTPMessage::HTTP_1_1);

                requestForm.prepareSubmit(webRequest);

                m_pClientSession->sendRequest(webRequest);

                Poco::Net::HTTPResponse webResponse;
                
                std::istream &responseChannel = m_pClientSession->receiveResponse(webResponse);

                if (webResponse.getStatus() == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
                    std::stringstream packetData;

                    packetData << responseChannel.rdbuf();
    
                    pPacket->FromJSON(packetData.str());

                    success = true;
                }
            }

            m_pClientSession->reset();

            OnPacketRead(pPacket, success);

            return success;
        }

        HTTPClientSessionSPtr WebContainer::CreateClientSession(const Poco::URI &uri)
        {
            HTTPClientSessionSPtr pClientSession = std::make_shared<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());

            return pClientSession;
        }
    }
}