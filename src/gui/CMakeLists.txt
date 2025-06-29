# Define a biblioteca "gui" a partir dos seus arquivos de código-fonte
# O CMake (com AUTOUIC=ON) irá processar o frontprincipal.ui automaticamente
add_library(gui
    #CAMINHO DO .h DO FRONTPRINCIPAL
    frontprincipal.cpp
    ../../include/gui/frontprincipal.h 
    frontprincipal.ui

    #CAMINHO DO .h DO CAMERAWORKER
    cameraworker.cpp
    ../../include/gui/cameraworker.h
)

# A biblioteca "gui" precisa saber onde encontrar seus próprios cabeçalhos e os do Qt
target_include_directories(gui PUBLIC
    # Permite que outros módulos encontrem os headers da GUI via 'include/gui'
    ../../include
)

# Linka a biblioteca "gui" com as bibliotecas do Qt que ela usa
target_link_libraries(gui PRIVATE
    Qt5::Widgets
)