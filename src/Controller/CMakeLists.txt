cmake_minimum_required(VERSION 3.25)
project(Controller)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(../base_station/DroneController)

add_executable(Controller main.cpp ../base_station/DroneController/DroneController.cpp)

target_link_libraries(Controller ${CURSES_LIBRARIES})
TARGET_LINK_LIBRARIES(Controller ${Boost_LIBRARIES} )
