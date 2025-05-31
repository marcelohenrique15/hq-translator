add_library(translation translator.cpp)
target_include_directories(translation PUBLIC ${CMAKE_SOURCE_DIR}/include)
# Caso adicione APIs ou bibliotecas externas, adicione os links aqui
