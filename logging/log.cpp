#pragma once

#include <include/log.hpp>
#include <include/date.hpp>

#include <exception>
#include <ios>
#include <iostream>
#include <fstream>

void Logger::CloseLogger() {
    log_file_.close();
}

Logger& Logger::GetLogger() {
    logger_ptr_ = new Logger();
    return *logger_ptr_; 
}

// Либо делать второй аргумент с ссылкой на дескриптор файла, либо делать свитч в PrepareLog. Кажется, что решение со свитчем - не очень задумка
// Со свитчем решение такое, что в зависимости от log_level я присваиваю std::ofstream& chosen_file один из info_file_, debug_file_ и т.д.
//      В этом случае +строки кода, +одна непонятная переменная.
//Со вторым аргументом всё проще, но нужно передавать второй аргумент. <-- Делаю так


void Logger::PrepareLog(const std::string& log_level) {
    log_file_ << std::endl << log_level << Helpers::GetDateAndTime();
}

std::ofstream& Logger::Info() {
    PrepareLog(kInfoLevel); 
    return log_file_;
}
std::ofstream& Logger::Debug() {
    PrepareLog(kDebugLevel);
    return log_file_;
}
std::ofstream& Logger::Warn() {
    PrepareLog(kWarnLevel);
    return log_file_;
}
std::ofstream& Logger::Error() {
    PrepareLog(kErrorLevel);
    return log_file_;
}

Logger::~Logger() {
    std::cout << "~Logger()";
    CloseLogger();
    delete logger_ptr_;
}

