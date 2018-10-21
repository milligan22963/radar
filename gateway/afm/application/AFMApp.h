/*--------------------------------------
  AFMApp
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#ifndef _H_AFM_APP
#define _H_AFM_APP

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Logger.h>

#include "common/Settings.h"

/**
 * @brief top level namespace for afm library assets
 */
namespace afm
{
    /**
     * @brief secondary level for application definitions
     */
    namespace application
    {
        /**
         * @class AFMApp
         * 
         * @brief base class for all AFM related applications to share from
         */
        class AFMApp : public Poco::Util::ServerApplication, public container::IContainerNotification,
            public std::enable_shared_from_this<AFMApp>
        {
            public:
                /**
                 * @brief Default constructor
                 */
                AFMApp();

                /**
                 * @brief constructor w/ app name for logging
                 */
                AFMApp(const std::string &appName);

                //-------------------------------------------------
                // IContainerNotification
                //-------------------------------------------------

                /**
                 * @copydoc IContainerNotification::OnPacketWritten
                 */
                virtual void OnPacketWritten(const std::string &containerId, const data::IDataPacketSPtr pPacket, bool success) override;

                /**
                 * @copydoc IContainerNotification::OnPacketRead
                 */
                virtual void OnPacketRead(const std::string &containerId, data::IDataPacketSPtr pPacket, bool success) override;

                /**
                 * @copydoc IContainerNotification::OnContainerReady
                 */
                virtual void OnContainerReady(const std::string &containerId, bool isReady) override;

                //-------------------------------------------------
                // END IContainerNotification
                //-------------------------------------------------

            protected:

                //-------------------------------------------------
                // Poco::Util::ServerApplication Overrides
                //-------------------------------------------------

                /**
                 * @copydoc Poco::Util::Application::initialize
                 */
                virtual void initialize(Application &self) override;

                /**
                 * @copydoc Poco::Util::Application::main
                 */
                virtual int main(const std::vector<std::string> &args) override;

                /**
                 * @copydoc Poco::Util::Application::defineOptions
                 */
                virtual void defineOptions(Poco::Util::OptionSet &options) override;

                /**
                 * @copydoc Poco::Util::Application::handleOption
                 */
                virtual void handleOption(const std::string &name, const std::string &value) override;

                //-------------------------------------------------
                // END Poco::Util::ServerApplication Overrides
                //-------------------------------------------------

                //-------------------------------------------------
                // Derived class overrides
                //-------------------------------------------------

                /**
                 * @brief called when the application starts up after all configuration has
                 * been done.
                 */
                virtual void OnStartup() = 0;

                /**
                 * @brief called when the application begins its tear down, prior to closing
                 * out the logger and any settings/containers
                 */
                virtual void OnShutdown() = 0;

                //-------------------------------------------------
                // END Derived class overrides
                //-------------------------------------------------

                /**
                 * @brief pauses the app while waiting for something to happen
                 */
                void Wait();
                std::cv_status WaitFor(std::chrono::steady_clock::duration waitTime);
                std::cv_status WaitUntil(std::chrono::system_clock::time_point waitTime);

                /**
                 * @brief Locks the mutex to syncrhonize operation
                 */
                void Lock();

                /**
                 * @brief Unlocks the mutex
                 */
                void UnLock();

                /**
                 * @brief Unlocks the mutex and notifies anyone sitting on the
                 *          condition variable
                 */
                void UnLockAndNotify();

                /**
                 * @brief returns the settings for the application
                 * 
                 * @return common::ISettingsSPtr which is the active settings
                 */
                common::ISettingsSPtr GetSettings() { return m_pSettings; }

                /**
                 * @brief returns an instance of the logger for this app
                 */
                Poco::Logger &GetLogger() { return m_logger; }

                /**
                 * @brief used by derived classes to know when shutdown
                 *          is in progress
                 * 
                 * @return true if shutting down, false otherwise
                 */
                inline bool IsShuttingDown() const { return m_shutdownInProgress; }

            private:
                /**
                 * @brief shuts down the application instance
                 */
                void Shutdown();

                /**
                 * @brief handles a configuration option
                 * 
                 * @param[in] name - the name of the option - should be configuration
                 * @param[in] value - the value corresponding to the option
                 */
                void HandleConfiguration(const std::string &name, const std::string &value);

            private:
                std::condition_variable m_condition;
                std::mutex m_conditionMutex;
                std::string m_configurationFile = "none";
                common::ISettingsSPtr m_pSettings = nullptr;
                Poco::Logger &m_logger;
                std::atomic<bool> m_shutdownInProgress;
        };
    }
}
#endif