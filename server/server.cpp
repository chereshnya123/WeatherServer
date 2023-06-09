#include <fastcgi++/request.hpp>
#include <fastcgi++/webstreambuf.hpp>
#include <fastcgi++/manager.hpp>
#include <soci/soci.h>

#include <string>

#include <src/db_working.cpp>
#include <src/weather_forecast.cpp>

std::string GetCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    return static_cast<std::string>(std::ctime(&end_time));

}

class HelloWorld: public Fastcgipp::Request<wchar_t>
{
  private:

    bool response()
    {
        std::cout << std::string(environment().requestUri.begin(),environment().requestUri.end())  << std::endl;
        soci::session sql(soci::postgresql, "dbname=weather");

        out << L"Content-Type: text/plain; charset=utf-8\r\n\r\n";

        weather::WeatherCast weather;
        const auto& weather_fetched = FetchOne(sql, "london");


        if (sql.got_data()) {
            std::cout << "Today weather is already in DB" << std::endl;
            weather = weather_fetched;
        } else {
            weather = weather::GetTodayWeatherForecast();
            InsertWeather(weather.city, weather.date, weather.humidity, weather.weather_state, weather.wind_speed, weather.temperature, sql);
            std::cout << "Successfuly put weather in DB" << std::endl;
        }

        out << "Today weather in " << weather.GetWCity() <<  "\n";
        out << "Date: " << weather.GetWDate() << "\n";
        out << "Temperature: " << weather.temperature << "\n";
        out << "Humidity: " << weather.humidity << "\n";
        out << "Wind speed: " << weather.wind_speed << "\n";
        out << "Weather state: " << weather.weather_state << "\n";
 

        return true;
    }
};
 
int main()
{
    Fastcgipp::Manager<HelloWorld> manager;



    manager.setupSignals();
    manager.listen("127.0.0.1", "9001");
    manager.start();



    manager.join();
 
    return 0;
}