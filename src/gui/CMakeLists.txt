# Cria a biblioteca 'gui'
add_library(gui STATIC
    frontprincipal.cpp
    frontprincipal.ui
)

target_include_directories(gui PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Garanta que 'services' está aqui e que a visibilidade é 'PUBLIC'.
target_link_libraries(gui PUBLIC
    services
    Qt5::Widgets
)