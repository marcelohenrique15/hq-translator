add_library(translation STATIC
translator.cpp
)

target_include_directories(translation PUBLIC
${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(translation
PRIVATE
cpr::cpr
nlohmann_json::nlohmann_json
)