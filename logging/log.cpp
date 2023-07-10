#include "include/date.hpp"
#include <include/log.hpp>

#include <string>


Logger& Logger::GetLogger() {
    static Logger instance = Logger();
    return instance;
}

Logger::Logger() : log_file_{"../logs/Log.txt"} {}

void Logger::PrepareLog(const std::string& log_level) {
    std::cout << log_level << Helpers::GetDateAndTime();
    log_file_ << log_level << Helpers::GetDateAndTime();
}

Logger& Logger::operator<<(const std::string& msg) {
    PrepareLog();
    std::cout << msg << std::endl;
    log_file_ << msg << std::endl; 

    return *this;
}