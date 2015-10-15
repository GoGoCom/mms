#include "Logging.h"

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"

INITIALIZE_EASYLOGGINGPP

namespace sim {

// Create and initialize the static variables
std::string Logging::m_runId = "";
std::string Logging::m_simLoggerName = "sim";
std::string Logging::m_mazeLoggerName = "maze";
std::string Logging::m_mouseLoggerName = "mouse";
std::map<std::string, std::pair<std::string, int>> Logging::m_info;

el::Logger* Logging::simLogger() {
    return getLogger(m_simLoggerName);
}

el::Logger* Logging::mazeLogger() {
    return getLogger(m_mazeLoggerName);
}

el::Logger* Logging::mouseLogger() {
    return getLogger(m_mouseLoggerName);
}

void Logging::initialize(const std::string& runId) {

    // Ensure we only initialize the loggers once
    static bool initialized = false;
    ASSERT(!initialized);
    initialized = true;

    // Set the runId
    m_runId = runId;

    // For each of the logger names ...
    for (std::string loggerName : {m_simLoggerName, m_mazeLoggerName, m_mouseLoggerName}) {
        
        // ... create the logger info ...
        std::string loggerPath = Directory::getRunDirectory() + m_runId + "/" + loggerName + "/default.txt";
        m_info.insert(std::make_pair(loggerName, std::make_pair(loggerPath, 1)));

        // ... and then create the logger ...
        el::Logger* logger = el::Loggers::getLogger(loggerName);

        // ... and then configure it
        el::Configurations loggerConfig;
        loggerConfig.setGlobally(el::ConfigurationType::Filename, loggerPath);
        loggerConfig.setGlobally(el::ConfigurationType::ToStandardOutput, "true");
        loggerConfig.setGlobally(el::ConfigurationType::MaxLogFileSize,
            std::to_string(10 * 24)); // 10 MiB, ~10,000 lines
        loggerConfig.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
        loggerConfig.setGlobally(el::ConfigurationType::Format,
            "[%datetime{%Y-%M-%d %H:%m:%s.%g}] # %logger # (%level) - %msg");
        el::Loggers::reconfigureLogger(logger, loggerConfig);
    }

    // Set some flags and the rollout callback
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Helpers::installPreRollOutCallback(rolloutHandler);
}

el::Logger* Logging::getLogger(const std::string& loggerName) {
    ASSERT(SimUtilities::mapContains(m_info, loggerName));
    return el::Loggers::getLogger(loggerName);
}

std::string Logging::getNextFileName(const char* filename) {
    std::string path = "";
    for (std::pair<std::string, std::pair<std::string, int>> info : m_info) {
        std::string loggerName = info.first;
        std::string loggerPath = info.second.first;
        int numLogFiles = info.second.second;
        if (std::string(filename) == loggerPath) {
            path = "/" + loggerName + "/" + std::to_string(numLogFiles) + ".txt";
            m_info.at(loggerName) = std::make_pair(loggerPath, numLogFiles + 1);
        }
    }
    ASSERT(path != "");
    return Directory::getRunDirectory() + m_runId + path;
}

void Logging::rolloutHandler(const char* filename, std::size_t size) {
    int value = std::rename(filename, getNextFileName(filename).c_str());
    ASSERT_EQUAL(value, 0);
}

} // namespace sim
