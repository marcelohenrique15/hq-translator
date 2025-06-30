cmake_minimum_required(VERSION 3.14)
project(hq_translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt5 MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Pacotes
find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(TESSERACT REQUIRED tesseract)
pkg_check_modules(ARCHIVE REQUIRED libarchive)

# Diretórios de includes
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}
    ${ARCHIVE_INCLUDE_DIRS}
)

# Diretórios das bibliotecas
link_directories(
    ${TESSERACT_LIBRARY_DIRS}
    ${ARCHIVE_LIBRARY_DIRS}
)

# CPR - HTTP Client
include(FetchContent)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.4
)
FetchContent_MakeAvailable(cpr)

# Submódulos
add_subdirectory(src/capture)
add_subdirectory(src/detection)
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)
add_subdirectory(src/services)
add_subdirectory(src/gui)

# Executável principal
add_executable(main
  main.cpp
  include/gui/frontprincipal.h
  include/services/visionservices.h
)

# Includes adicionais
target_include_directories(main PRIVATE
    ${TESSERACT_INCLUDE_DIRS}
    ${ARCHIVE_INCLUDE_DIRS}
)

# Linkagem
target_link_libraries(main
    capture
    detection
    overlayer
    translation
    services
    gui
    Qt5::Widgets
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    cpr::cpr
    ${TESSERACT_LIBRARIES}
    ${ARCHIVE_LIBRARIES}
    archive  # <- forçando libarchive
    -llept
)
