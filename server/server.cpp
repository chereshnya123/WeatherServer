#include <fastcgi++/request.hpp>
#include <fastcgi++/webstreambuf.hpp>
#include <fastcgi++/manager.hpp>
#include <soci/soci.h>

#include <string>

#include <include/db_working.hpp>
#include <include/weather_forecast.hpp>
#include <include/log.hpp>

class HelloWorld: public Fastcgipp::Request<wchar_t>
{
  private:

    bool response()
    {
        std::cout << std::string(environment().requestUri.begin(),environment().requestUri.end())  << std::endl;
        soci::session sql(soci::postgresql, "dbname=weather");

        out << L"Content-Type: text/plain; charset=utf-8\r\n\r\n";

        Weather::WeatherCast weather;
        const auto& weather_fetched = FetchOne(sql, "london");

        std::cout << "prik";
        if (sql.got_data()) {
            LOG.Info() << "Today weather is already in DB";
            std::cout << "Today weather is already in DB" << std::endl;
            weather = weather_fetched;
        } else {
            std::cout << "prik";
            weather = Weather::GetTodayWeatherForecast();
            InsertWeather(weather.city, weather.date, weather.humidity, weather.weather_state, weather.wind_speed, weather.temperature, sql);
            LOG.Info() << "Successfuly put weather in DB";
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