#pragma once


#include <soci/session.h>
#include <soci/soci-backend.h>
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>

#include <src/weather_forecast.cpp>

#include <iostream>

void TestDB () {

    soci::session sql_session(soci::postgresql, "dbname=weather");

    std::cout << "Soci is included now" << std::endl;

}

void TestSelect(soci::session& sql) {
    sql << "SELECT * FROM London;";
}

weather::WeatherCast FetchOne(soci::session& sql, const std::string city = "london") {
    weather::WeatherCast weather;
    std::cout << "Fetching ...";
    sql << "SELECT date, humidity, state, wind_speed, temperature FROM " << city, soci::into(weather.date), soci::into(weather.humidity), soci::into(weather.weather_state), soci::into(weather.wind_speed), soci::into(weather.temperature);    
    std::cout << "Fetched";

    return weather;

}

void InsertWeather(const std::string& city, const std::string& date, int humidity,
    int weather_state, int wind_speed, int temperature, soci::session& sql) {
    
    sql << "INSERT INTO " << city << "(date, humidity, state, wind_speed, temperature)"
     << "VALUES ( '" << date << "', " << humidity << "," << weather_state << "," << wind_speed << ", " << temperature << ");";


}