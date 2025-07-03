add_library(detection STATIC text_detector.cpp)

target_include_directories(detection PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/struct
    ${CMAKE_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${TESSERACT_INCLUDE_DIRS}
    ${LEPTONICA_INCLUDE_DIRS}
)

target_link_libraries(detection PUBLIC
    ${OpenCV_LIBS}
    ${TESSERACT_LIBRARIES}
    ${LEPTONICA_LIBRARIES}
    -ljpeg
    -lpng
    -ltiff
    -lz
    -lopenjp2
    -lgif # <-- ADICIONE ESTA LINHA AQUI!
    struct_utils
)