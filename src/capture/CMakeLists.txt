add_library(capture webcam_capture.cpp)
target_include_directories(capture PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})
target_link_libraries(capture ${OpenCV_LIBS})
