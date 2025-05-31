cmake_minimum_required(VERSION 3.10)
project(hq_translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV
find_package(OpenCV REQUIRED)

# Incluir diretórios públicos
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# Subdiretórios de módulos
add_subdirectory(src/capture)
add_subdirectory(src/detection)
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)

# Executável principal
add_executable(main main.cpp)
target_link_libraries(main
    capture
    detection
    overlayer
    translation
    ${OpenCV_LIBS}
)
