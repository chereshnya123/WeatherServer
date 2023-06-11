#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <chrono>


namespace Helpers {

namespace {
    std::string MakeTwoDigits(int day) {
        ++day;
        if (day < 10)
            return "0" + std::to_string(day);
        else 
            return std::to_string(day);
    }
}

// Да-да, здесь дублируется код. Потом исправлю

std::string GetTodayDate() {

    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&end_time);
    std::stringstream date;
    date << MakeTwoDigits(local_tm.tm_mday) << ".";
    date << MakeTwoDigits(local_tm.tm_mon) << ".";
    date << local_tm.tm_year  + 1900;
    std::cout << "Date: " << date.str();
    return date.str();
}

std::string GetDateAndTime() {

    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&end_time);
    std::stringstream date_and_time;
    date_and_time << "[" << MakeTwoDigits(local_tm.tm_hour) << ":";
    date_and_time << MakeTwoDigits(local_tm.tm_min) << ":";
    date_and_time << MakeTwoDigits(local_tm.tm_sec) << "|";
    date_and_time << MakeTwoDigits(local_tm.tm_mday) << "/";
    date_and_time << MakeTwoDigits(local_tm.tm_mon) << "/";
    date_and_time << local_tm.tm_year  + 1900 << "]";
    return date_and_time.str();
}

}