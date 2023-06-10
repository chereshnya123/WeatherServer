#pragma once

#include <chrono>

namespace Helpers {

namespace {
    std::string MakeTwoDigits(int day) {
        if (day < 10)
            return "0" + std::to_string(day);
        else 
            return std::to_string(day);
    }
}

std::string GetTodayDate() {

    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&end_time);
    std::stringstream date;
    date << MakeTwoDigits(local_tm.tm_mday) << ".";
    date << MakeTwoDigits(local_tm.tm_mon) << ".";
    date << local_tm.tm_year  + 1900;
    std::cout << "Date: " << date.str() << std::endl;
    return date.str();
}

}