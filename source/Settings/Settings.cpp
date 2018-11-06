#include "Settings/Settings.hpp"

namespace simplelogger::settings {
    Settings::Settings(std::string path) 
        :
            m_impl(std::unique_ptr<SettingsImpl>())
    {

    }
}