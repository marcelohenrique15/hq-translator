cmake_minimum_required(VERSION 3.14)
project(hq_translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pacotes necessários
find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(TESSERACT REQUIRED tesseract)

# Diretórios de include
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}
)

# Diretórios de bibliotecas (link)
link_directories(${TESSERACT_LIBRARY_DIRS})

# CPR (HTTP client)
include(FetchContent)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.4
)
FetchContent_MakeAvailable(cpr)

# Subdiretórios dos módulos
add_subdirectory(src/capture)
add_subdirectory(src/detection)
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)
add_subdirectory(src/threads)


# Executável principal
add_executable(main main.cpp)

# Includes adicionais (ex: para Tesseract/Leptonica)
target_include_directories(main PRIVATE
    ${TESSERACT_INCLUDE_DIRS}
)

# Link final com bibliotecas principais
target_link_libraries(main
  capture
  detection
  overlayer
  translation
  threads
  ${OpenCV_LIBS}
  nlohmann_json::nlohmann_json
  cpr::cpr
  ${TESSERACT_LIBRARIES}
  -llept
)
