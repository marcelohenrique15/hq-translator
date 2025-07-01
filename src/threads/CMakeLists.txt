add_library(threads STATIC
    webcam_thread.cpp     
    detection_thread.cpp     
    translate_thread.cpp 
)  

target_include_directories(threads PRIVATE     
    ${CMAKE_SOURCE_DIR}     
    ${CMAKE_SOURCE_DIR}/include     
    ${OpenCV_INCLUDE_DIRS} 
)

target_link_libraries(threads PUBLIC 
    detection 
    capture 
    translation 
    overlayer
    ${OpenCV_LIBS}
)