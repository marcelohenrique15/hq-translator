cmake_minimum_required(VERSION 3.14)
project(hq_translator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Adiciona CXX flags para tentar mitigar problemas de ABI (se for o caso)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -D_GLIBCXX_USE_CXX11_ABI=0") # Tente isso se persistir com [abi:cxx11]

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

# Modulos
add_subdirectory(src/capture)
add_subdirectory(src/detection) # <<< ESTE AQUI
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)

# Executavel principal
add_executable(main main.cpp)

# Garante que o linker encontre a libdetection.a
target_link_directories(main PRIVATE
    $<TARGET_FILE_DIR:detection> # O diretorio de build da libdetection
)

target_link_libraries(main
  capture
  detection # <<< LINKA COM A BIBLIOTECA 'detection'
  overlayer
  translation
  ${OpenCV_LIBS} # OpenCV para main e todas as dependencias de OpenCV (incluindo DNN se usada)
  nlohmann_json::nlohmann_json
  cpr::cpr
)