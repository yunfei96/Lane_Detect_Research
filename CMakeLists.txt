cmake_minimum_required(VERSION 2.8)
project( Lane_Detect_Research )

#require a C++11 compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find OpenCV
find_package( OpenCV REQUIRED )

#set Src 
#-----------------------add new files in the src in future implementation----------------
set(src
  preprocess.cpp
  preprocess.hpp
  function.cpp
)
#------------------------should not modify below---------------------------------------
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( detect detect.hpp detect_video.cpp ${src} )
target_link_libraries( detect ${OpenCV_LIBS} )