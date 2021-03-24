cmake_minimum_required(VERSION 3.3)
project(tenniscourt)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(include)
file(GLOB SOURCE_FILES "src/*.cpp")
add_executable(hawkeye ${SOURCE_FILES})
target_link_libraries(hawkeye ${OpenCV_LIBS})
