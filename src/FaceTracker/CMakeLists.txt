cmake_minimum_required(VERSION 3.25)
project(FaceTracker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(OpenCV REQUIRED)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(../base_station/DroneController)

add_executable(FaceTracker main.cpp ../base_station/DroneController/DroneController.cpp)

target_link_libraries(FaceTracker ${CURSES_LIBRARIES})
target_link_libraries(FaceTracker ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(FaceTracker ${Boost_LIBRARIES} )