cmake_minimum_required(VERSION 3.14)
project(hq_translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(TESSERACT REQUIRED tesseract)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}
)
link_directories(${TESSERACT_LIBRARY_DIRS})

include(FetchContent)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.4
)
FetchContent_MakeAvailable(cpr)

# módulos
add_subdirectory(src/capture)
add_subdirectory(src/detection)
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)

# executável principal
add_executable(main main.cpp)
target_link_libraries(main
  capture
  detection
  overlayer
  translation
  ${OpenCV_LIBS}
  nlohmann_json::nlohmann_json
  cpr::cpr
)

