#include <fastcgi++/request.hpp>
#include <fastcgi++/webstreambuf.hpp>
#include <fastcgi++/manager.hpp>
#include <soci/soci.h>

#include <iostream>
#include <string>
#include <fstream>

#include <src/db_working.cpp>
#include <src/weather_forecast.cpp>


class HelloWorld: public Fastcgipp::Request<wchar_t>
{
  private:

    bool response()
    {
        std::cout << "got req" << std::endl;
        std::cout << std::string(environment().requestUri.begin(),environment().requestUri.end())  << std::endl;
        // soci::session sql(soci::postgresql, "dbname=weather");
        std::ifstream code{"/home/chereshnya/nauchka/top52/duplicates/delete_full_dups.rb"};
        std::string code_str{};
        out << L"Content-Type: text/plain; charset=utf-8\r\n\r\n";
        out << L"Success with request\n";
        code >> code_str;
        out << std::wstring(code_str.begin(), code_str.end());
        std::cout << code_str;
        // weather::WeatherCast weather;
        // const auto& weather_fetched = FetchOne(sql, "london");


        // if (sql.got_data()) {
        //     std::cout << "Today weather is already in DB" << std::endl;
        //     weather = weather_fetched;
        // } else {
        //     weather = weather::GetTodayWeatherForecast();
        //     InsertWeather(weather.city, weather.date, weather.humidity, weather.weather_state, weather.wind_speed, weather.temperature, sql);
        //     std::cout << "Successfuly put weather in DB" << std::endl;
        // }

        // out << "Today weather in " << weather.GetWCity() <<  "\n";
        // out << "Date: " << weather.GetWDate() << "\n";
        // out << "Temperature: " << weather.temperature << "\n";
        // out << "Humidity: " << weather.humidity << "\n";
        // out << "Wind speed: " << weather.wind_speed << "\n";
        // out << "Weather state: " << weather.weather_state << "\n";
 

        return true;
    }
};
 
int main()
{
    Fastcgipp::Manager<HelloWorld> manager;


    std::cout << "Init\n";
    manager.setupSignals();
    manager.listen("127.0.0.1", "9001");
    manager.start();



    manager.join();
 
    return 0;
}