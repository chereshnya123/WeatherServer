#pragma once

#include <exception>
#include <ios>
#include <iostream>
#include <fstream>

#include <src/helpers/date.cpp>

const std::string& kLogFile{"../logs/Log.txt"};

const std::string& kInfoLevel{"[INFO]"};
const std::string& kDebugLevel{"[DEBUG]"};
const std::string& kWarnLevel{"[WARN]"};
const std::string& kErrorLevel{"[ERORR]"};

class Logger {

public:

    void CloseLogger() {
        log_file_.close();
    }

    static Logger& GetLogger() {
        if (!logger_ptr_)
            logger_ptr_ = new Logger();
        return *logger_ptr_; 
    }

// Либо делать второй аргумент с ссылкой на дескриптор файла, либо делать свитч в PrepareLog. Кажется, что решение со свитчем - не очень задумка
// Со свитчем решение такое, что в зависимости от log_level я присваиваю std::ofstream& chosen_file один из info_file_, debug_file_ и т.д.
//      В этом случае +строки кода, +одна непонятная переменная.
//Со вторым аргументом всё проще, но нужно передавать второй аргумент. <-- Делаю так


    void PrepareLog(const std::string& log_level) {
        log_file_ << std::endl << log_level << Helpers::GetDateAndTime();
    }

    std::ofstream& Info() {
        PrepareLog(kInfoLevel); 
        return log_file_;
    }
    std::ofstream& Debug() {
        PrepareLog(kDebugLevel);
        return log_file_;
    }
    std::ofstream& Warn() {
        PrepareLog(kWarnLevel);
        return log_file_;
    }
    std::ofstream& Error() {
        PrepareLog(kErrorLevel);
        return log_file_;
    }

private: 

    Logger(const Logger&) = delete;
    Logger() = default;
    ~Logger() {
        std::cout << "~Logger()";
        CloseLogger();
        delete logger_ptr_;
    }

    std::ofstream log_file_{kLogFile};

    static Logger* logger_ptr_;

};

Logger* Logger::logger_ptr_ = nullptr;

Logger& LOG = Logger::GetLogger();