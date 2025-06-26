cmake_minimum_required(VERSION 3.14)
project(hq_translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# <<< Habilita as funcionalidades automáticas do CMake para Qt (MOC, UIC, RCC).
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)
#Busca o Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

include(FetchContent)

FetchContent_Declare(
cpr
GIT_REPOSITORY https://github.com/libcpr/cpr.git
GIT_TAG 1.10.4
)

FetchContent_MakeAvailable(cpr)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

add_subdirectory(src/capture)
add_subdirectory(src/detection)
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)
add_subdirectory(src/gui)

add_executable(main main.cpp)

target_link_libraries(main
capture
detection
overlayer
translation
#biblioteca módulo GUI
gui
#bibliotecas do Qt5
Qt5::Widgets
${OpenCV_LIBS}
nlohmann_json::nlohmann_json
)
