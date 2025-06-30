# Cria a biblioteca estática 'services' a partir dos seus arquivos fonte.
# <<< CORREÇÃO 2: Padronizamos o nome para o singular >>>
add_library(services STATIC
    visionservices.cpp
)

# Informa ao módulo 'services' que ele pode encontrar cabeçalhos
# na pasta 'include' da raiz do projeto.
target_include_directories(services PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Vincula a biblioteca 'services' com as outras bibliotecas de
# backend que ela utiliza para funcionar.
target_link_libraries(services PUBLIC
    # Nossas bibliotecas internas
    capture
    detection
    overlayer
    translation
    
    # Dependências externas
    ${OpenCV_LIBS}
    ${TESSERACT_LIBRARIES}
    -llept
    cpr::cpr
    nlohmann_json::nlohmann_json
    Qt5::Core
    Qt5::Gui
)