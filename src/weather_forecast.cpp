#include <cpr/cpr.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ostream>
#include <sstream>
#include <string>
#include <optional>
#include <sstream>

#include <src/helpers/date.cpp>
#include <logging/log.cpp>

#include <include/log.hpp>
#include <include/weather_forecast.hpp>
#include <include/date.hpp>
// To-Parse list:
// temp ------------------- OK
// weather state  // Parse HTML with gumbo
// humidity ----------------- OK
// wind_speed --------------------- OK

static auto& LOG = Logger::GetLogger();

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
}


int Weather::GetIndexOfFirstNumber(const std::string& text, const int begin, const int end) {
    std::cout << "left = " << begin << " right = " << end << std::endl;
    std::cout << "Searching in: " << std::endl << text << std::endl;
    for (int i = begin; i < end; ++i) {
        std::cout << "Cycled\n";
        std::cout << text[i];
        if (text[i] == '-' || isdigit(text[i]))
            return i;
    }
    throw -1;
    return -1;
}

std::optional<std::string> Weather::ParseValueFromHTML(const std::string& text, const std::string& tag) {
    int left = text.find(tag) + tag.size() + 1;
    int right = left + kValueSize;
    left = Weather::GetIndexOfFirstNumber(text, left, right);
    for ( int i = left; i < right; ++i) {
        if (!std::isdigit(text[i])) {
            right = i;
            return text.substr(left, right);
        }
    }

    return std::nullopt;
}

int Weather::ParseTemperature(const std::string& text) {
    LOG << "Parsing temperature...";
    //std::cout << "Searching IN: " << std::endl << text;
    const auto& parsed_temperature = Weather::ParseValueFromHTML(text, kCurrentTemperaturePattern);

    if (!parsed_temperature.has_value()) {
        LOG << "ERROR: Can't parse temperature, return default: 100";
        return 100;
    }

    const auto& temperature = std::stoi(parsed_temperature.value());
    LOG << "Temperature parsed successfuly";
    return temperature;
}

double Weather::ParseWindSpeed(const std::string& text) { // std::optional

    LOG << "Parsing wind speed";
    const auto& parsed_wind_speed = Weather::ParseValueFromHTML(text, kWindSpeedPattern);

    if (!parsed_wind_speed.has_value()) {
        LOG << "ERROR: can't parse wind speed, return default: 0";
    }

    const auto& wind_speed = std::stod(parsed_wind_speed.value());
    LOG << "Wind speed parsed successfuly";
    return wind_speed;
}

std::string Weather::ParseWeatherState (const std::string& text) {
    const auto& parsed_weather_state = Weather::ParseValueFromHTML(text, kWindSpeedPattern);

    if (!parsed_weather_state.has_value()) {
        throw "Can't parse Weather state";
    }

    const auto& weather_state = parsed_weather_state.value();

    return weather_state;
}

int Weather::ParseHumidity (const std::string& text) {
    const auto& parsed_humidity = Weather::ParseValueFromHTML(text, kWindSpeedPattern);

    if (!parsed_humidity.has_value()) {
        throw "Can't parse Weather state";
    }

    const auto& humidity = std::stoi(parsed_humidity.value());

    return humidity;
}


// Req: Добавить кэш страниц, чтобы не ждать, пока скачается страница, которая у меня уже есть
// Response: Сделано с помощью БД


std::ostream&  operator<< (std::ostream& out, const Weather::WeatherCast& weather_cast) {
        out << "Weather forecast:" << std::endl;
        out << "City: " << weather_cast.city << std::endl;
        out << "Date: " << weather_cast.date << std::endl;
        out << "Temp: " << weather_cast.temperature << std::endl;

        return out;
}



Weather::WeatherCast Weather::ParseWeatherForecast(const std::string& text) {
    WeatherCast weather_forecast;

    LOG << "Parsing weather forecast...";
    weather_forecast.temperature = Weather::ParseTemperature(text);
    weather_forecast.city = "london";
    weather_forecast.wind_speed = Weather::ParseWindSpeed(text);
    weather_forecast.humidity = 56; // Заглушка 

    weather_forecast.date = Helpers::GetTodayDate();

    return weather_forecast;
}

Weather::WeatherCast Weather::GetTodayWeatherForecast(std::string city) {

    cpr::Response r = cpr::Get(cpr::Url{"https://yandex.ru/pogoda/" + city},
                      cpr::Parameters{{"Content-Type", kContentType}, {"Accept", kAccept},
                      {"Accept-Encoding", "identity"},
                      {"User-Agent", kUserAgent}});
    // r.status_code;                  // 200 Оформить в логи
    // r.header["content-type"];       // application/json; charset=utf-8
    LOG << "Got response";
    std::transform(city.begin(), city.end(), city.begin(), tolower);
    WeatherCast weather_forecast;
    weather_forecast = Weather::ParseWeatherForecast(r.text);
    weather_forecast.city = city;
    // WeatherCast weather_cast{city, "04.06.2023", today_temperature};
    LOG << "Returend weather";
    return weather_forecast;
}

Weather::WeatherCast::operator std::string() {
    std::stringstream weather_forecast;
    weather_forecast << "Weather Forecast : \n";
    weather_forecast << "City: " << city << "\n";
    weather_forecast << "Date: " << date << "\n";
    weather_forecast << "Temp: " << temperature << std::endl;

    return weather_forecast.str();
}