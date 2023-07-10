#pragma once

#include <string>
#include <iostream>
#include <optional>

namespace Weather {

int GetIndexOfFirstNumber(const std::string& text, const int begin, const int end);

std::optional<std::string> ParseValueFromHTML(const std::string& text, const std::string& tag);

int ParseTemperature(const std::string& text);

double ParseWindSpeed(const std::string& text);

std::string ParseWeatherState (const std::string& text);

int ParseHumidity (const std::string& text);

struct WeatherCast {
    std::string city = "london";
    std::string date = "now";
    int temperature = 123;
    int weather_state = 1;
    double wind_speed = 123;
    int humidity = 1;

    operator std::string();
    friend std::ostream& operator<< (std::ostream& out, const WeatherCast& weather_cast);
    std::wstring GetWDate() const {return std::wstring(date.begin(), date.end());}
    std::wstring GetWCity() const {return std::wstring(city.begin(), city.end());}
};

std::ostream& operator<< (std::ostream& out, const WeatherCast& weather_cast);
WeatherCast ParseWeatherForecast(const std::string& text);
WeatherCast GetTodayWeatherForecast(std::string city = "london");



}
