cmake_minimum_required(VERSION 3.14) # Define a versão mínima do CMake necessária para o projeto.
project(hq_translator) # Define o nome do projeto CMake.

set(CMAKE_CXX_STANDARD 17) # Define o padrão C++ a ser usado (C++17).
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Garante que o padrão C++17 seja estritamente respeitado.

# Pacotes necessários

find_package(OpenCV REQUIRED)   # Procura a biblioteca OpenCV (obrigatória).
find_package(nlohmann_json CONFIG REQUIRED) # Procura a biblioteca nlohmann/json usando seu arquivo de configuração (obrigatória).
find_package(PkgConfig REQUIRED) # Procura o PkgConfig, uma ferramenta para gerenciar bibliotecas (obrigatória).

# Tesseract e Leptonica

pkg_check_modules(TESSERACT REQUIRED tesseract) # Usa PkgConfig para encontrar a biblioteca Tesseract.
pkg_check_modules(LEPTONICA REQUIRED lept) # Usa PkgConfig para encontrar a biblioteca Leptonica (dependência do Tesseract).

# CPR (HTTP client)
# Inclui o módulo FetchContent para baixar e integrar bibliotecas externas (cpr neste caso).
include(FetchContent)
FetchContent_Declare(
  cpr # Nome da dependência.
  GIT_REPOSITORY https://github.com/libcpr/cpr.git # Repositório Git do cpr.
  GIT_TAG 1.10.4  # Tag específica para garantir uma versão fixa.
)
FetchContent_MakeAvailable(cpr) # Baixa e disponibiliza a biblioteca cpr.

# Biblioteca de cabeçalhos para struct/
# Define uma biblioteca INTERFACE. Bibliotecas INTERFACE não produzem arquivos de biblioteca,
# mas são usadas para propagar propriedades (como include directories) para targets que as usam.

add_library(struct_utils INTERFACE)

# Adiciona o diretório 'struct' como um diretório de inclusão para 'struct_utils'.
# Qualquer target que linkar com 'struct_utils' herdará essa propriedade.

target_include_directories(struct_utils INTERFACE ${CMAKE_SOURCE_DIR}/struct)
# Subdiretórios dos módulos
# Adiciona os diretórios dos sub-módulos, onde cada um pode ter seu próprio CMakeLists.txt.

add_subdirectory(src/capture)
add_subdirectory(src/detection)
add_subdirectory(src/overlayer)
add_subdirectory(src/translation)
add_subdirectory(src/threads)

# Executável principal
add_executable(main main.cpp) # Define o executável principal do projeto a partir de main.cpp.

# Includes específicos do executável
# Adiciona diretórios de inclusão privados para o executável 'main'.
# Estes são os diretórios onde o compilador procurará por arquivos de cabeçalho.

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/include # Diretório global de includes do projeto.
    ${TESSERACT_INCLUDE_DIRS}  # Diretórios de includes do Tesseract.
    ${LEPTONICA_INCLUDE_DIRS}  # Diretórios de includes do Leptonica.
    ${OpenCV_INCLUDE_DIRS}  # Diretórios de includes do OpenCV.
)

# Link com bibliotecas
# Linka o executável 'main' com as bibliotecas necessárias.

target_link_libraries(main 
  threads # Linka com a biblioteca 'threads' (criada em src/threads/CMakeLists.txt).
  detection  # Linka com a biblioteca 'detection'.
  capture # Linka com a biblioteca 'capture'.
  overlayer # Linka com a biblioteca 'overlayer'.
  translation # Linka com a biblioteca 'translation'.
  struct_utils # Linka com a biblioteca 'struct_utils' (interface para includes).
  nlohmann_json::nlohmann_json # Linka com a biblioteca JSON.
  cpr::cpr # Linka com a biblioteca CPR.
  ${OpenCV_LIBS} # Linka com as bibliotecas do OpenCV.
  # A ordem é crucial. As bibliotecas que _usam_ outras vêm antes.
  ${TESSERACT_LIBRARIES} # Linka com as bibliotecas do Tesseract. Tesseract depende de Leptonica e LibArchive.
  ${LEPTONICA_LIBRARIES} # Linka com as bibliotecas do Leptonica. Leptonica depende de outras libs como WebP.
  -larchive  # Linka com a libarchive para dependências do Tesseract.
  -ljpeg  # Linka com a biblioteca JPEG.
  -lpng  # Linka com a biblioteca PNG.
  -ltiff # Linka com a biblioteca TIFF.
  -lz # Linka com a biblioteca zlib (compressão).
  -lopenjp2 # Linka com a biblioteca OpenJPEG2.
  -lgif # Linka com a biblioteca GIF.
  -lwebp # Linka com a biblioteca WebP.
)