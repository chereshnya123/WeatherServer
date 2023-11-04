project(WeatherParser)
cmake_minimum_required(VERSION 3.22)
set (CMAKE_CXX_STANDARD 17)

find_package(cpr REQUIRED)
find_package(SOCI REQUIRED)
add_executable(weather_server server/server.cpp)

target_include_directories(weather_server PRIVATE 
    /usr/include/postgresql
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
