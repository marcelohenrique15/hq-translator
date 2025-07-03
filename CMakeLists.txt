cmake_minimum_required(VERSION 3.14)
project(hq_translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pacotes necessários
find_package(OpenCV REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)

# Tesseract e Leptonica
pkg_check_modules(TESSERACT REQUIRED tesseract)
pkg_check_modules(LEPTONICA REQUIRED lept)

# CPR (HTTP client)
include(FetchContent)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.4
)
FetchContent_MakeAvailable(cpr)

# Biblioteca de cabeçalhos para struct/
add_library(struct_utils INTERFACE)
target_include_directories(struct_utils INTERFACE ${CMAKE_SOURCE_DIR}/struct)

# Subdiretórios dos módulos
add_subdirectory(src/capture)
add_subdirectory(src/detection)
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)
add_subdirectory(src/threads)

# Executável principal
add_executable(main main.cpp)

# Includes específicos do executável
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${TESSERACT_INCLUDE_DIRS}
    ${LEPTONICA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

# Link com bibliotecas
target_link_libraries(main
  threads
  detection
  capture
  overlayer
  translation
  struct_utils
  nlohmann_json::nlohmann_json
  cpr::cpr
  ${OpenCV_LIBS}
  # A ordem é crucial. As bibliotecas que _usam_ outras vêm antes.
  ${TESSERACT_LIBRARIES}
  ${LEPTONICA_LIBRARIES}
  -ljpeg
  -lpng
  -ltiff
  -lz
  -lopenjp2
  -lgif # <-- ADICIONE ESTA LINHA AQUI!
)