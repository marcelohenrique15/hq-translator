# Define uma biblioteca estática chamada 'threads' a partir dos arquivos fonte.
add_library(threads STATIC
    webcam_thread.cpp # Arquivo fonte para a thread de captura da webcam.
    detection_thread.cpp # Arquivo fonte para a thread de detecção de texto.
    translate_thread.cpp # Arquivo fonte para a thread de tradução.
)  

# Especifica os diretórios de inclusão privados para a biblioteca 'threads'.
# Estes são necessários para compilar os arquivos fonte desta biblioteca.

target_include_directories(threads PRIVATE     
    ${CMAKE_SOURCE_DIR}  # Diretório raiz do projeto.
    ${CMAKE_SOURCE_DIR}/include # Diretório global de includes.
    ${OpenCV_INCLUDE_DIRS} # Diretórios de includes do OpenCV.
)

# Especifica as bibliotecas públicas que a biblioteca 'threads' linka.
# Isso significa que qualquer target que linkar com 'threads' também precisará dessas bibliotecas.

target_link_libraries(threads PUBLIC 
    detection # Linka com a biblioteca 'detection'.
    capture  # Linka com a biblioteca 'capture'.
    translation  # Linka com a biblioteca 'capture'.
    overlayer # Linka com a biblioteca 'overlayer'.
    ${OpenCV_LIBS} # Linka com as bibliotecas do OpenCV.
)