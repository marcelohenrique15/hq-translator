cmake_minimum_required(VERSION 3.10)
project(visao-computacional)

find_package(OpenCV REQUIRED)

add_executable(visao-computacional main.cpp)
target_include_directories(visao-computacional PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(visao-computacional ${OpenCV_LIBS})