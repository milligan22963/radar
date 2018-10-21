/*--------------------------------------
  AFMApp
  
  Company: AFM Software
  Copyright: 2018
  --------------------------------------*/

#include "AFMApp.h"

namespace afm
{
    namespace application
    {
        // options
        static const std::string sc_configurationFile[] = {
            "configuration",
            "cfg",
            "The configuration file for this application"
        };
        
        AFMApp::AFMApp()
            : m_logger(Poco::Logger::get("AFMApp"))
            , m_shutdownInProgress(false)
        {

        }

        AFMApp::AFMApp(const std::string &appName)
            : m_logger(Poco::Logger::get(appName))
            , m_shutdownInProgress(false)
        {

        }


        //-------------------------------------------------
        // IContainerNotification
        //-------------------------------------------------

        void AFMApp::OnPacketWritten(const std::string &containerId, const data::IDataPacketSPtr pPacket, bool success)
        {

        }

        void AFMApp::OnPacketRead(const std::string &containerId, data::IDataPacketSPtr pPacket, bool success)
        {
        }

        void AFMApp::OnContainerReady(const std::string &containerId, bool isReady)
        {
        }

        //-------------------------------------------------
        // END IContainerNotification
        //-------------------------------------------------

        //protected:

        //-------------------------------------------------
        // Poco::Util::ServerApplication Overrides
        //-------------------------------------------------
        void AFMApp::initialize(Application &self)
        {
            ServerApplication::initialize(self);
        }

        int AFMApp::main(const std::vector<std::string> &args)
        {
            m_logger.information("\nApplication is starting");

            OnStartup();

            waitForTerminationRequest();

            m_logger.information("\nApplication is shutting down");

            Shutdown();

            return 0;
        }

        void AFMApp::defineOptions(Poco::Util::OptionSet &options)
        {
            ServerApplication::defineOptions(options);

            options.addOption(
                Poco::Util::Option(sc_configurationFile[0], sc_configurationFile[1],
                        sc_configurationFile[2])
                    .required(true)
                    .repeatable(false)
                    .argument("file")
                    .callback(Poco::Util::OptionCallback<AFMApp>(this, &AFMApp::HandleConfiguration)));  
        }

        void AFMApp::handleOption(const std::string &name, const std::string &value)
        {
            ServerApplication::handleOption(name, value);
        }

        //-------------------------------------------------
        // END Poco::Util::ServerApplication Overrides
        //-------------------------------------------------

        void AFMApp::Shutdown()
        {
            m_shutdownInProgress = true;

            Lock();
            
            OnShutdown();

            m_pSettings->Shutdown();
            m_pSettings = nullptr;

            UnLock();
        }

        void AFMApp::HandleConfiguration(const std::string &name, const std::string &value)
        {
            m_configurationFile = value;
            m_logger.information("Using configuration file: %s", value);

            m_pSettings = std::make_shared<common::Settings>();

            if (m_pSettings->Initialize(m_configurationFile, shared_from_this()) == false) {
                m_logger.error("Unable to initialize settings");
            }
        }

        void AFMApp::Wait()
        {
            std::unique_lock<std::mutex> lk(m_conditionMutex);

            m_condition.wait(lk);
        }

        std::cv_status AFMApp::WaitFor(std::chrono::steady_clock::duration waitTime)
        {
            std::unique_lock<std::mutex> lk(m_conditionMutex);

            return m_condition.wait_for(lk, waitTime);
        }

        std::cv_status AFMApp::WaitUntil(std::chrono::system_clock::time_point waitTime)
        {
            std::unique_lock<std::mutex> lk(m_conditionMutex);

            return m_condition.wait_until(lk, waitTime);
        }

        void AFMApp::Lock()
        {
            m_conditionMutex.lock();
        }

        void AFMApp::UnLock()
        {
            m_conditionMutex.unlock();
        }

        void AFMApp::UnLockAndNotify()
        {
            m_conditionMutex.unlock();
            m_condition.notify_one();
        }
    }
}
