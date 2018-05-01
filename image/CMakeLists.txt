cmake_minimum_required (VERSION 2.6)
project (VideoMaker)
set (CMAKE_PREFIX_PATH "/usr/local/share/OpenCV")
find_package (OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(VideoMaker main.cpp)
target_link_libraries( VideoMaker ${OpenCV_LIBS} )

