# src/translation/CMakeLists.txt
add_library(translation STATIC
    LibreTranslate.cpp
)

# Adicione esta linha para que translate.cpp encontre translate.h
target_include_directories(translation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Esta linha já estava correta para expor os includes para outros targets
target_include_directories(translation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(translation
    PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
)