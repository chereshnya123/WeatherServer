#pragma once

#include <exception>
#include <ios>
#include <iostream>
#include <fstream>

#include <include/date.hpp>

namespace {

const std::string& kLogFile{"../logs/Log.txt"};

const std::string& kInfoLevel{"[INFO]"};
const std::string& kDebugLevel{"[DEBUG]"};
const std::string& kWarnLevel{"[WARN]"};
const std::string& kErrorLevel{"[ERORR]"};

}

class Logger {
  public:
    
    void CloseLogger();

    static Logger& GetLogger();
    void PrepareLog(const std::string& log_level);
    std::ofstream& Info();
    std::ofstream& Debug();
    std::ofstream& Warn();
    std::ofstream& Error();
  private:

    std::ofstream log_file_{kLogFile};
    static Logger* logger_ptr_;
    
    Logger(const Logger&) = delete;
    Logger();
    ~Logger() = default;
    
};

// Logger* Logger::logger_ptr_ = nullptr;

