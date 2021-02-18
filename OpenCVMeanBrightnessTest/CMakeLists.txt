cmake_minimum_required(VERSION 3.16)
project(OpenCVMeanBrightnessTest)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(OpenCVMeanBrightnessTest main.cpp HelperFunctions.h)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})