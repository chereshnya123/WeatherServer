project(WeatherParser)
cmake_minimum_required(VERSION 3.22)
set (CMAKE_CXX_STANDARD 17)

# find_package(cpr REQUIRED)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

find_package(SOCI REQUIRED)
add_executable(weather_server server/server.cpp 
                include/date.hpp
                include/db_working.hpp
                include/weather_forecast.hpp
                include/log.hpp
                src/db_working.cpp
                src/weather_forecast.cpp
                src/helpers/date.cpp
                logging/log.cpp)

target_include_directories(weather_server PRIVATE 
    /usr/include/postgresql
    /home/chereshnya/libs/fastcgi++/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(weather_server
    PRIVATE cpr::cpr
    fastcgipp
    SOCI::soci_core
    SOCI::soci_postgresql
)

enable_testing()
add_executable(tester tester.cpp)
add_test(simple_test tester)
