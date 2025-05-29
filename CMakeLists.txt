cmake_minimum_required(VERSION 3.10)

project(visao-computacional)

set(CMAKE_CXX_STANDARD 17)

# Encontre o OpenCV
find_package(OpenCV REQUIRED)

# Inclui os headers do OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Adiciona o executável
add_executable(visao-computacional main.cpp)

# Linka o OpenCV
target_link_libraries(visao-computacional ${OpenCV_LIBS})
