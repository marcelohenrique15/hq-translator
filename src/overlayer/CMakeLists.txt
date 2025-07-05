# Define uma biblioteca (por padrão, estática) chamada 'overlayer' a partir do arquivo fonte 'overlay_renderer.cpp'.
add_library(overlayer overlay_renderer.cpp)

# Especifica os diretórios de inclusão públicos para a biblioteca 'overlayer'.
# Isso significa que qualquer outro módulo que use 'overlayer' terá acesso a esses diretórios para encontrar cabeçalhos.
# - ${CMAKE_SOURCE_DIR}/include: O diretório global de includes do projeto.
# - ${CMAKE_SOURCE_DIR}: O diretório raiz do projeto.

target_include_directories(overlayer PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})

# Especifica as bibliotecas com as quais a biblioteca 'overlayer' linka.
# Neste caso, ela linka com as bibliotecas do OpenCV, que são necessárias para as funcionalidades de renderização.

target_link_libraries(overlayer ${OpenCV_LIBS})
