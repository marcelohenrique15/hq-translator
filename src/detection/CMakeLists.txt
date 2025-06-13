add_library(detection text_detector.cpp)
target_include_directories(detection PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(detection ${OpenCV_LIBS})
add_executable(test_tesseract 
    test_tesseract.cpp
    text_detector.cpp
)

target_link_libraries(test_tesseract
    ${TESSERACT_LIBRARIES}
    ${OpenCV_LIBS}
)