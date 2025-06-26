# src/detection/CMakeLists.txt

# Adiciona a biblioteca 'detection' e seus arquivos fonte
add_library(detection STATIC
    text_detector.cpp
)

# Define os diretórios de inclusao publicos
# Isso permite que outros targets que linkam com 'detection' encontrem seus cabeçalhos.
target_include_directories(detection PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Linka as dependencias OpenCV (para DNN, ImgProc) e Tesseract com a biblioteca 'detection'.
# As dependencias devem ser explicitas aqui.
target_link_libraries(detection
    PRIVATE
        ${OpenCV_LIBS} # Contem modulos como dnn, imgproc
        ${TESSERACT_LIBRARIES} # Contem tesseract
)

# Embora ja declarado no .h, esta eh a definicao da variavel no escopo do modulo
# Para garantir visibilidade. Se as versoes de GCC sao diferentes, isso pode causar ABI issues.
# Mas aqui, estamos apenas reforçando que ela faz parte do modulo detection.
# Nada a mudar no seu text_detector.cpp para isso.