#pragma once

#include <include/db_working.hpp>

#include <soci/session.h>
#include <soci/soci-backend.h>
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>

#include <include/log.hpp>
#include <include/date.hpp>
#include <iostream>

namespace Test{
void TestDB () {
    soci::session sql_session(soci::postgresql, "dbname=weather");

    std::cout << "Soci is included now" << std::endl;

}

void TestSelect(soci::session& sql) {
    sql << "SELECT * FROM London;";
}
}

std::string TodayDateQuery(const std::string& query) {
    return query + " WHERE date = '" + Helpers::GetTodayDate() + "'";
}

Weather::WeatherCast FetchOne(soci::session& sql, const std::string city = "london") {
    Weather::WeatherCast weather;
    sql << "SELECT date, humidity, state, wind_speed, temperature FROM " << city 
        << " WHERE date = '" << Helpers::GetTodayDate() << "';", soci::into(weather.date), soci::into(weather.humidity), soci::into(weather.weather_state), soci::into(weather.wind_speed), soci::into(weather.temperature);    
    std::cout << "Successfuly fetched one record" << std::endl;
    LOG.Info() << "Successfuly fetched one record";
    

    return weather;

}

void InsertWeather(const std::string& city, const std::string& date, int humidity,
    int weather_state, int wind_speed, int temperature, soci::session& sql) {
    sql.once << "INSERT INTO " << city << "(date, humidity, state, wind_speed, temperature)"
     << "VALUES ( '" << date << "', " << humidity << "," << weather_state << "," << wind_speed << ", " << temperature << ")";


}