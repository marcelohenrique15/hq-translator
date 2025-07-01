add_library(detection STATIC text_detector.cpp)

target_include_directories(detection PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(detection PRIVATE
        ${OpenCV_LIBS}
        ${TESSERACT_LIBRARIES} 
)
