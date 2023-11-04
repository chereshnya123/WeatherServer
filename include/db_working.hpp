#pragma once

#include <soci/session.h>
#include <soci/soci-backend.h>
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>

#include <include/weather_forecast.hpp>
#include <include/date.hpp>
#include <include/log.hpp>
#include <iostream>

std::string TodayDateQuery(const std::string& query);

void InsertWeather(const std::string& city, const std::string& date, int humidity,
    int weather_state, int wind_speed, int temperature, soci::session& sql);

Weather::WeatherCast FetchOne(soci::session& sql, const std::string city);