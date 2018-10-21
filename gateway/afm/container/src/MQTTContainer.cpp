/*--------------------------------------
  MQTTContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include <iostream>
#include <string.h>

#include <Poco/MD5Engine.h>

#include "SystemDefines.h"

#include "common/SystemFactory.h"
#include "data/DataPacketFactory.h"
#include "MQTTContainer.h"

namespace afm
{
    namespace container
    {
        /**
         * Async mqtt callbacks
         */
        void ConnectionLost(void *pContext, char *pCause)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnConnectionLost();
            }
        }

        void Connected(void *pContext, char *pResponse)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnConnected();
            } else {
                std::cout << "No container to talk to.\n";
            }
        }

        void ConnectFailure(void *pContext, MQTTAsync_failureData *pResponse)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnConnectionFailure();
            }
        }
        
        void Disconnected(void *pContext, MQTTAsync_successData *pResponse)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnDisconnected();
            }
        }

        int MessageArrived(void *pContext, char *pTopicName, int topicLen, MQTTAsync_message *pMessage)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                data::IDataPacketSPtr pPacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::DATA);

                std::string incomingMessage;

                incomingMessage.copy((char *)pMessage->payload, pMessage->payloadlen);

                pPacket->FromJSON(incomingMessage);

                std::string topic;

                // per docs, if 0 then null terminated
                if (topicLen == 0) {
                    topic = pTopicName;
                } else {
                    topic.copy(pTopicName, topicLen);
                }

                pPacket->SetValue(sc_topic, topic);

                pContainer->OnMessageArrived(pPacket);
            }

            return 0;
        }

        void MessageSent(void *pContext, MQTTAsync_successData *pResponse)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnMessageSent(pResponse->token);
            }
        }

        void MessageFail(void *pContext, MQTTAsync_failureData *pFailureData)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnMessageFail(pFailureData->token);
            }
        }

        void DeliveryComplete(void *pContext, MQTTAsync_token token)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnDeliveryComplete(token);
            }
        }

        void SubscriptionMade(void *pContext, MQTTAsync_successData *pResponse)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnSubscriptionReady();
            }
        }

        void SubscriptionFailed(void *pContext, MQTTAsync_failureData *pResponse)
        {
            MQTTContainer *pContainer = (MQTTContainer *)pContext;

            if (pContainer != nullptr) {
                pContainer->OnSubscriptionFailed();
            }
        }

        /**
         * MQTT Class proper
         */
        MQTTContainer::MQTTContainer()
            : Container()
        {

        }

        MQTTContainer::~MQTTContainer()
        {
            Reset(); // clear everything out
        }

        bool MQTTContainer::Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient)
        {
            Reset();

            bool success = Container::Initialize(options, pNotificationRecipient);

            if (success == true) {

                ProcessOptions(options);

                common::ISystemSPtr pSystem = common::SystemFactory::GetSystem();
                Poco::MD5Engine md5Engine;

                std::string clientId = pSystem->GetSerialNumber();

                clientId += GetTarget();

                // if the user provided a client id then tack that on
                if (m_clientId.empty() == false) {
                    GetLogger().information("Setting up mqtt client: %s", m_clientId);
                    clientId += m_clientId;
                }

                md5Engine.update(clientId.c_str(), clientId.length());

                clientId = Poco::DigestEngine::digestToHex(md5Engine.digest());

                GetLogger().information("ClientId: %s", clientId);

                std::string hostAddress = "ssl";
                std::string hostPort = "8883";

                if (m_sslEnabled == false) {
                    hostAddress = "tcp";
                    hostPort = "1883";
                }
                
                hostAddress += "://" + GetTarget() + ":" + hostPort;

                GetLogger().information("Connecting to: %s", hostAddress);

                char *pLocation = nullptr;
                
                if (m_persistenceType != MQTTCLIENT_PERSISTENCE_NONE) {
                    int16_t bufferSize = m_persistenceLocation.length() + clientId.length() + 2;

                    pLocation = new char[bufferSize];

                    snprintf(pLocation, bufferSize, "%s/%s", m_persistenceLocation.c_str(), clientId.c_str());
                }

                MQTTAsync_createOptions createOptions = MQTTAsync_createOptions_initializer;

                createOptions.sendWhileDisconnected = m_maxBufferedMessages == -1 ? 0 : 1;
                createOptions.maxBufferedMessages = m_maxBufferedMessages > 0 ? m_maxBufferedMessages : 0;

                if (MQTTAsync_createWithOptions(&m_client, hostAddress.c_str(), clientId.c_str(), m_persistenceType, (void *)pLocation, &createOptions) == MQTTASYNC_SUCCESS) {
                    MQTTAsync_setCallbacks(m_client, this, ConnectionLost, MessageArrived, DeliveryComplete);
	                MQTTAsync_setConnected(m_client, this, Connected);

                    MQTTAsync_connectOptions connectionOptions = MQTTAsync_connectOptions_initializer;

                    connectionOptions.keepAliveInterval = m_keepAliveInterval;
                    connectionOptions.cleansession = m_cleanSession == true ? 1 : 0;
                    //connectionOptions.onSuccess = Connected; handled by the setConnected call above
                    connectionOptions.onFailure = ConnectFailure;
                    connectionOptions.automaticReconnect = m_autoReconnect == true ? 1 : 0;
                    connectionOptions.context = this;

                    MQTTAsync_SSLOptions sslOptions = MQTTAsync_SSLOptions_initializer;

                    if (m_sslEnabled == true) {
                        sslOptions.sslVersion = MQTT_SSL_VERSION_TLS_1_2;
                        sslOptions.verify = 1;
                        sslOptions.CApath = nullptr; // need to check this
                        if (m_sslTrustStore.empty() == false) {
                            sslOptions.trustStore = m_sslTrustStore.c_str();
                        }
                        if (m_sslPrivateCertificate.empty() == false) {
                            sslOptions.privateKey = m_sslPrivateCertificate.c_str();
                        }
                        if (m_sslPrivatePassphrase.empty() == false) {
                            sslOptions.privateKeyPassword = m_sslPrivatePassphrase.c_str();
                        }
                        if (m_sslPublicCertificate.empty() == false) {
                            sslOptions.keyStore = m_sslPublicCertificate.c_str();
                        }
                        connectionOptions.ssl = &sslOptions;
                    }

                    if (MQTTAsync_connect(m_client, &connectionOptions) != MQTTASYNC_SUCCESS) {
                        // failed to call connect
                        GetLogger().error("Unable to connect to: %s", GetTarget());
                    } else {
                        GetLogger().information("Initial connection made");
                    }
                } else {
                    GetLogger().error("Unable to create client: %s", m_clientId);
                }

                if (pLocation != nullptr) {
                    delete [] pLocation;
                }

                m_sslPrivatePassphrase.clear(); // clear out so not in memory

                success = true;
            }

            return success;
        }

        bool MQTTContainer::WritePacket(const data::IDataPacketSPtr pPacket)
        {
            bool success = false;

            if (m_isConnected == true) {
                std::string topic;

                if (pPacket->GetValue(sc_topic, topic) == true) {
                    data::IDataPacketSPtr pOutgoingPacket = data::DataPacketFactory::CreateDataPacket(data::DataPacketType::DATA);

                    pOutgoingPacket->Clone(pPacket);

                    pOutgoingPacket->RemoveValue(sc_topic);

                    int16_t qos = 0;
                    bool retained = false;

                    std::string value;

                    if (pPacket->GetValue(sc_retain, value) == true) {
                        if (value == sc_true) {
                            retained = true;
                        }
                        pOutgoingPacket->RemoveValue(sc_retain);
                    }

                    // packet specific qos?
                    if (pPacket->GetValue(sc_qos, value) == true) {
                        qos = ::atoi(value.c_str());
                        pOutgoingPacket->RemoveValue(sc_qos);
                    }

                    std::string message = pOutgoingPacket->ToJSON();

                    MQTTAsync_responseOptions responseOptions = MQTTAsync_responseOptions_initializer;
                    MQTTAsync_message outgoingMessage = MQTTAsync_message_initializer;

                    responseOptions.onSuccess = MessageSent;
                    responseOptions.onFailure = MessageFail;
                    responseOptions.context = this;

                    char *pMessage = new char[message.length() + 1];
                    
                    snprintf(pMessage, message.length() + 1, "%s", message.c_str());

                    outgoingMessage.payload = pMessage;
                    outgoingMessage.payloadlen = message.length();
                    outgoingMessage.qos = qos;
                    outgoingMessage.retained = retained == true ? 1 : 0;
                    int messageError = MQTTAsync_sendMessage(m_client, topic.c_str(), &outgoingMessage, &responseOptions);
                    if (messageError != MQTTASYNC_SUCCESS) {
                        GetLogger().error("Failed to send message: %d", messageError);
                    }
                    delete [] pMessage;
                } else {
                    GetLogger().error("Unable to send packet - no topic defined");
                }
            }

            OnPacketWritten(pPacket, success);

            return success;
        }

        /**
         * Dynamic callbacks for MQTT processing
         */

        void MQTTContainer::OnConnectionLost()
        {
            m_isConnected = false;
            
            SetIsReady(m_isConnected);

            GetLogger().error("Connection lost to MQTT");
        }

        void MQTTContainer::OnConnected()
        {
            m_isConnected = true;

            if (m_topics.empty() == false) {
                MQTTAsync_responseOptions subscribeOptions = MQTTAsync_responseOptions_initializer;

                subscribeOptions.onSuccess = SubscriptionMade;
                subscribeOptions.onFailure = SubscriptionFailed;
                subscribeOptions.context = this;

                int topicCount = 0;
                char **pTopics = new char*[m_topics.size()];
                int *pQOS = new int[m_topics.size()];

                for (auto topic : m_topics) {
                    //
                    pTopics[topicCount] = new char[topic.length() + 1];
                    snprintf(pTopics[topicCount], topic.length() + 1, "%s", topic.c_str());
                    
                    pQOS[topicCount] = m_qos;

                    topicCount++;
                }

                if (MQTTAsync_subscribeMany(m_client, m_topics.size(), pTopics, pQOS, &subscribeOptions) != MQTTASYNC_SUCCESS) {
                    GetLogger().error("Subscription failed on call");
                }

                delete [] pQOS;
                delete [] pTopics;
            } else {
                // if no topics then we are ready
                SetIsReady(m_isConnected);
            }
            GetLogger().information("Connected to MQTT");
        }

        void MQTTContainer::OnConnectionFailure()
        {
            m_isConnected = false;

            SetIsReady(m_isConnected);

            GetLogger().error("Connection failure to MQTT");
        }

        void MQTTContainer::OnDisconnected()
        {
            m_isConnected = false;

            SetIsReady(m_isConnected);

            GetLogger().information("Disconnected to MQTT");
        }

        void MQTTContainer::OnSubscriptionReady()
        {
            SetIsReady(true);
        }

        void MQTTContainer::OnSubscriptionFailed()
        {
            SetIsReady(false);
        }

        void MQTTContainer::OnMessageArrived(data::IDataPacketSPtr pPacket)
        {
            if (OnPacketRead(pPacket, true) == false) {
                // squirrled away in the base then...
            }
        }

        void MQTTContainer::OnMessageSent(int32_t messageId)
        {
            // were we expecting it?
        }

        void MQTTContainer::OnMessageFail(int32_t messageId)
        {
            // Notify sender that it failed
        }

        void MQTTContainer::OnDeliveryComplete(int32_t messageId)
        {
        }
        
        /*
            Internal parts
         */
        void MQTTContainer::ProcessOptions(const nlohmann::json &options)
        {
            if (options.find(sc_useSSL) != options.end()) {
                m_sslEnabled = options[sc_useSSL].get<bool>();
            }
            if (options.find(sc_clientId) != options.end()) {
                m_clientId = options[sc_clientId].get<std::string>();
            }
            if (options.find(sc_topics) != options.end()) {
                for (auto topic : options[sc_topics]) {
                    m_topics.insert(topic.get<std::string>());
                }
            }
            if (options.find(sc_qos) != options.end()) {
                m_qos = options[sc_qos].get<int16_t>();
            }
            if (options.find(sc_sslPublicCertificate) != options.end()) {
                m_sslPublicCertificate = options[sc_sslPublicCertificate];
            }
            if (options.find(sc_sslPrivateCertificate) != options.end()) {
                m_sslPrivateCertificate = options[sc_sslPrivateCertificate];
            }
            if (options.find(sc_sslTrustStore) != options.end()) {
                m_sslTrustStore = options[sc_sslTrustStore];
            }
            if (options.find(sc_sslPrivatePassphrase) != options.end()) {
                m_sslPrivatePassphrase = options[sc_sslPrivatePassphrase];
            }
            if (options.find(sc_keepAliveInterval) != options.end()) {
                m_keepAliveInterval = options[sc_keepAliveInterval].get<uint32_t>();
            }
            if (options.find(sc_cleanSession) != options.end()) {
                m_cleanSession = options[sc_cleanSession].get<bool>();
            }
            if (options.find(sc_persistence) != options.end()) {
                if (options[sc_persistence] == sc_persistenceNone) {
                    m_persistenceType = MQTTCLIENT_PERSISTENCE_NONE;
                } else if (options[sc_persistence] == sc_persistenceDefault) {
                    m_persistenceType = MQTTCLIENT_PERSISTENCE_DEFAULT;
                }
            }
            if (options.find(sc_persistenceLocation) != options.end()) {
                m_persistenceLocation = options[sc_persistenceLocation];
            }
            if (options.find(sc_autoReconnect) != options.end()) {
                m_autoReconnect = options[sc_autoReconnect].get<bool>();
            }
            if (options.find(sc_maxBufferedMessages) != options.end()) {
                int32_t messageCount = options[sc_maxBufferedMessages].get<int32_t>();
                m_maxBufferedMessages = messageCount > 0 ? messageCount : -1;
            }
        }

        void MQTTContainer::Reset()
        {
            m_sslEnabled = false;
            m_cleanSession = false;
            m_autoReconnect = true;
            m_keepAliveInterval = 30;
            m_persistenceType = MQTTCLIENT_PERSISTENCE_NONE;
            m_persistenceLocation.clear();
            m_sslPrivateCertificate.clear();
            m_sslPrivatePassphrase.clear();
            m_sslPublicCertificate.clear();
            m_sslTrustStore.clear();

            m_isConnected = false;
        }
    }
}
