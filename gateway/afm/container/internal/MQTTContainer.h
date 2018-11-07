/*--------------------------------------
  MQTTContainer

  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_MQTTCONTAINER
#define _H_MQTTCONTAINER

#include <cstdint>
#include <set>
#include <map>
#include <mutex>

extern "C" {
    #include <paho/MQTTAsync.h>
}
#include "Container.h"

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
         * Well known fields for options
         */
        const std::string sc_useSSL = "useSSL";
        const std::string sc_sslPublicCertificate = "sslPublicCert";
        const std::string sc_sslPrivateCertificate = "sslPrivateCert";
        const std::string sc_sslTrustStore = "sslTrustStore";
        const std::string sc_sslPrivatePassphrase = "sslPassPhrase";
        const std::string sc_clientId = "clientId";
        const std::string sc_keepAliveInterval = "keepAlive";
        const std::string sc_cleanSession = "cleanSession";
        const std::string sc_maxBufferedMessages = "bufferedMessages";
        const std::string sc_persistence = "persistence";
        const std::string sc_persistenceLocation = "persistenceLocation";
        const std::string sc_autoReconnect = "autoReconnect";

        /**
         * Well known fields for packet specifics
         */
        const std::string sc_topics = "topics";
        const std::string sc_topic = "topic";
        const std::string sc_retain = "retain";
        const std::string sc_qos = "qos";

        // persistence types
        const std::string sc_persistenceNone = "none";
        const std::string sc_persistenceDefault = "default";

        /**
         * @class MQTTContainer
         * 
         * @brief base implementation for MQTT containers
         */
        class MQTTContainer : public Container
        {
            public:
                /**
                 * @brief default constructor
                 */
                MQTTContainer();

                /**
                 * @brief default descructor
                 */
                virtual ~MQTTContainer();

                /**
                 * @copydoc IContainer::Initialize
                 * 
                 * target will be the broker adress
                 */
                virtual bool Initialize(const ContainerOptions &options, IContainerNotificationSPtr pNotificationRecipient = nullptr) override;

                /**
                 * @copydoc IContainer::WritePacket
                 */
                virtual bool WritePacket(const data::IDataPacketSPtr pPacket) override;

                /**
                 * Dynamic callbacks for MQTT processing
                 */

                /**
                 * @brief called when the connection to the MQTT broker is lost
                 */
                void OnConnectionLost();

                /**
                 * @brief called when the connection to the MQTT broker is established
                 */
                void OnConnected();

                /**
                 * @brief called when the connection to the MQTT broker is not established
                 */
                void OnConnectionFailure();

                /**
                 * @brief called when the connection to the MQTT broker is disconnected
                 */
                void OnDisconnected();

                /**
                 * @brief called when a message is received from the broker
                 */
                void OnMessageArrived(data::IDataPacketSPtr pPacket);

                /**
                 * @brief called when a message has been sent
                 * 
                 * @param[in] messageId - the id of the message (token) that was sent
                 */
                void OnMessageSent(int32_t messageId);

                /**
                 * @brief called when a message has failed to be sent
                 * 
                 * @param[in] messageId - the id of the message (token) that failed
                 */
                void OnMessageFail(int32_t messageId);

                /**
                 * @brief called when a message has been delivered
                 * 
                 * @param[in] messageId - the id of the message (token) that was delivered
                 */
                void OnDeliveryComplete(int32_t messageId);

                /**
                 * @brief called when the subscription(s) are made
                 */
                void OnSubscriptionReady();

                /**
                 * @brief called when the subscription(s) fails
                 */
                void OnSubscriptionFailed();

            protected:
                /**
                 * @brief process the options for the given container
                 * 
                 * @param[in] optionSet
                 */
                virtual void ProcessOptions(const nlohmann::json &options);

                /**
                 * @brief connects to the mqtt broker
                 * 
                 * @return true on success, false on failure
                 */
                bool ConnectToBroker();

                /**
                 * @brief resets to initial values
                 */
                void Reset();

            private:
                MQTTAsync m_client;
                std::string m_clientId;
                bool m_sslEnabled = false;
                std::string m_sslPublicCertificate;
                std::string m_sslPrivateCertificate;
                std::string m_sslTrustStore;
                std::string m_sslPrivatePassphrase;

                int16_t m_qos = 0; // qos for all topics
                uint32_t m_keepAliveInterval = 30; // default to 30 seconds
                int32_t m_maxBufferedMessages = -1; // if positive then send while offline
                bool m_cleanSession = false;
                bool m_autoReconnect = true;
                int32_t m_persistenceType = MQTTCLIENT_PERSISTENCE_NONE;
                std::string m_persistenceLocation;

                std::mutex m_messageMutex;
                std::set<std::string> m_topics;

                bool m_isConnected = false;
        };
    }
}
#endif