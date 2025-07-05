# Cria uma biblioteca chamada 'capture' a partir do arquivo fonte 'webcam_capture.cpp'
add_library(capture webcam_capture.cpp)

# Define os diretórios de inclusão (headers) que serão visíveis publicamente para quem usar essa biblioteca
# - ${CMAKE_SOURCE_DIR}/include: diretório onde estão os headers (como webcam_capture.h)
# - ${CMAKE_SOURCE_DIR}: raiz do projeto (pode conter headers auxiliares ou ser necessário para builds maiores)

target_include_directories(capture PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})

# Liga a biblioteca 'capture' às bibliotecas do OpenCV
# Isso garante que todas as dependências do OpenCV sejam corretamente conectadas

target_link_libraries(capture ${OpenCV_LIBS})
