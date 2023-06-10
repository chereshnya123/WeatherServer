#pragma once

#include <cpr/cpr.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <string>
#include <optional>
#include <sstream>

#include <gumbo.h>

#include <src/helpers/date.cpp>

// To-Parse list:
// temp ------------------- OK
// weather state  // Parse HTML with gumbo
// humidity ----------------- OK
// wind_speed --------------------- OK

namespace weather {

namespace {

const std::string kContentType{"text/html; charset=utf-8"};
const std::string kAccept{"text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"};
const std::string kUserAgent{"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.193 Safari/537.36"};
const std::string kFactTemperatureClass{"temp__value temp__value_with-unit"};
const std::string kCurrentTemperaturePattern{"Текущая температура"};
const std::string kWindSpeedPattern{">Ветер:"};
const std::string kSunny{"Sunny"};
const std::string kCloudy{"Cloudy"};
const std::string kRain{"Rain"};
const std::string kSnowy{"Snowy"};
const std::string kDull{"Dull"};

constexpr int kValueSize = 15;



int GetIndexOfFirstNumber(const std::string& text, const int begin, const int end) {
    for (int i = begin; i < end; ++i) {
        if (text[i] == '-' || isdigit(text[i]))
            return i;
    }
    throw -1;
    return -1;
}

std::optional<std::string> ParseValueFromHTML(const std::string& text, const std::string& tag) {
    int left = text.find(tag) + tag.size() + 1;
    int right = left + kValueSize;
    left = GetIndexOfFirstNumber(text, left, right);
    for ( int i = left; i < right; ++i) {
        if (!std::isdigit(text[i])) {
            right = i;
            return text.substr(left, right);
        }
    }

    return std::nullopt;
}

int ParseTemperature(const std::string& text) {
    std::cout << "Parsing temperature...\n";
    const auto& parsed_temperature = ParseValueFromHTML(text, kCurrentTemperaturePattern);

    if (!parsed_temperature.has_value()) {
        std::cout << "ERROR: Can't parse temperature, return default: 100\n";
        return 100;
    }

    const auto& temperature = std::stoi(parsed_temperature.value());
    std::cout << "Temperature parsed successfuly\n";
    return temperature;
}

double ParseWindSpeed(const std::string& text) { // std::optional
    std::cout << "Parsing wind speed";
    const auto& parsed_wind_speed = ParseValueFromHTML(text, kWindSpeedPattern);

    if (!parsed_wind_speed.has_value()) {
        std::cout << "ERROR: can't parse wind speed, return default: 0\n";
        return 0;
    }

    const auto& wind_speed = std::stod(parsed_wind_speed.value());
    std::cout << "Wind speed parsed successfuly\n";
    return wind_speed;
}

std::string ParseWeatherState (const std::string& text) {
    const auto& parsed_weather_state = ParseValueFromHTML(text, kWindSpeedPattern);

    if (!parsed_weather_state.has_value()) {
        throw "Can't parse Weather state";
    }

    const auto& weather_state = parsed_weather_state.value();

    return weather_state;
}

int ParseHumidity (const std::string& text) {
    const auto& parsed_humidity = ParseValueFromHTML(text, kWindSpeedPattern);

    if (!parsed_humidity.has_value()) {
        throw "Can't parse Weather state";
    }

    const auto& humidity = std::stoi(parsed_humidity.value());

    return humidity;
}

} // namespace


// Req: Добавить кэш страниц, чтобы не ждать, пока скачается страница, которая у меня уже есть
// Response: Сделано с помощью БД

struct WeatherCast {
    std::string city = "london";
    std::string date = "now";
    int temperature = 123;
    int weather_state = 1;
    double wind_speed = 123;
    int humidity = 1;


    std::wstring GetWDate() const {return std::wstring(date.begin(), date.end());}
    std::wstring GetWCity() const {return std::wstring(city.begin(), city.end());}
};

std::ostream&  operator<< (std::ostream& out, const WeatherCast& weather_cast) {
        out << "Weather forecast:" << std::endl;
        out << "City: " << weather_cast.city << std::endl;
        out << "Date: " << weather_cast.date << std::endl;
        out << "Temp: " << weather_cast.temperature << std::endl;

        return out;
}



WeatherCast ParseWeatherForecast(const std::string& text) {
    WeatherCast weather_forecast;

    std::cout << "Parsing weather forecast...\n";
    weather_forecast.temperature = ParseTemperature(text);
    weather_forecast.city = "london";
    weather_forecast.wind_speed = ParseWindSpeed(text);
    weather_forecast.humidity = 56; // Заглушка 

    weather_forecast.date = Helpers::GetTodayDate();

    return weather_forecast;
}

WeatherCast GetTodayWeatherForecast(std::string city = "london") {
    std::cout << "In func\n";
    cpr::Response r = cpr::Get(cpr::Url{"https://yandex.ru/pogoda/" + city},
                      cpr::Parameters{{"Content-Type", kContentType}, {"Accept", kAccept},
                      {"Accept-Encoding", "identity"},
                      {"User-Agent", kUserAgent}});
    // r.status_code;                  // 200 Оформить в логи
    // r.header["content-type"];       // application/json; charset=utf-8
    std::cout << "Got response" << std::endl;
    std::transform(city.begin(), city.end(), city.begin(), tolower);
    WeatherCast weather_forecast;

    weather_forecast = ParseWeatherForecast(r.text);
    weather_forecast.city = city;
    // WeatherCast weather_cast{city, "04.06.2023", today_temperature};
    std::cout << "Returend weather" << std::endl;
    return weather_forecast;
}

} //namespace weather
