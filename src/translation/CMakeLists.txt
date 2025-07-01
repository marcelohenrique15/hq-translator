add_library(translation STATIC
    LibreTranslate.cpp
)

target_include_directories(translation PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_include_directories(translation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(translation
    PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
)
