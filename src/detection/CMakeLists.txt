# Define uma biblioteca estática chamada 'detection' a partir do arquivo fonte 'text_detector.cpp'.
add_library(detection STATIC text_detector.cpp)

# Especifica os diretórios de inclusão públicos para a biblioteca 'detection'.
# Estes diretórios contêm arquivos de cabeçalho necessários para compilar o módulo de detecção
# e para outros módulos que dependem de 'detection'.

target_include_directories(detection PUBLIC
    ${CMAKE_SOURCE_DIR}/include # Diretório global de includes do projeto.
    ${CMAKE_SOURCE_DIR}/struct # Diretório para estruturas de dados compartilhadas.
    ${CMAKE_SOURCE_DIR} # Raiz do diretório de código-fonte.
    ${OpenCV_INCLUDE_DIRS} # Diretórios de inclusão para OpenCV.
    ${TESSERACT_INCLUDE_DIRS} # Diretórios de inclusão para Tesseract OCR.
    ${LEPTONICA_INCLUDE_DIRS}  # Diretórios de inclusão para Leptonica (uma dependência do Tesseract).
)

# Especifica as bibliotecas públicas com as quais a biblioteca 'detection' linka.
# Estas são as bibliotecas das quais 'detection' depende e também
# bibliotecas que outros módulos que linkam com 'detection' precisarão.

target_link_libraries(detection PUBLIC
    ${OpenCV_LIBS}  # Linka com as bibliotecas do OpenCV.
    ${TESSERACT_LIBRARIES} # Linka com as bibliotecas do Tesseract.
    ${LEPTONICA_LIBRARIES} # Linka com as bibliotecas do Leptonica.
    -ljpeg # Linka com a biblioteca JPEG.
    -lpng  # Linka com a biblioteca PNG.
    -ltiff # Linka com a biblioteca TIFF.
    -lz # Linka com a biblioteca zlib (compressão).
    -lopenjp2 # Linka com a biblioteca OpenJPEG2.
    -lgif  # Linka com a biblioteca GIF (observação adicionada conforme o comentário original).
    struct_utils # Linka com a biblioteca de interface 'struct_utils' (para struct_data.h).
)