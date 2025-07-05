# Define uma biblioteca estática chamada 'translation' a partir do arquivo fonte 'LibreTranslate.cpp'.
add_library(translation STATIC
    LibreTranslate.cpp
)

# Especifica os diretórios de inclusão privados para a biblioteca 'translation'.
# Estes cabeçalhos são necessários apenas para a compilação interna desta biblioteca.

target_include_directories(translation PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Especifica os diretórios de inclusão públicos para a biblioteca 'translation'.
# É feita uma distinção entre os caminhos durante a fase de construção e após a instalação.

target_include_directories(translation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> # Caminho durante a fase de construção.
    $<INSTALL_INTERFACE:include> # Caminho após a instalação do projeto.
)

# Especifica as bibliotecas privadas com as quais a biblioteca 'translation' linka.
# Estas são dependências da 'translation' que não precisam ser expostas a outros módulos
# que porventura linkem com 'translation'.

target_link_libraries(translation
    PRIVATE
        cpr::cpr # Linka com a biblioteca cpr (C++ Requests), usada para requisições HTTP.
        nlohmann_json::nlohmann_json # Linka com a biblioteca Nlohmann JSON, usada para manipulação de JSON.
)
