cmake_minimum_required(VERSION 3.6)
project(PencilSketch)
find_package( OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories("/usr/include/eigen3/")
set(SOURCE_FILES main.cpp stroke.h stroke.cpp tone.h tone.cpp base.h textual.h)
add_executable(PencilSketch ${SOURCE_FILES})
target_link_libraries(PencilSketch ${OpenCV_LIBS} )