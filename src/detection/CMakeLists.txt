add_library(detection STATIC text_detector.cpp)

target_include_directories(detection PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(detection PRIVATE
        ${OpenCV_LIBS}
        ${TESSERACT_LIBRARIES} 
)
