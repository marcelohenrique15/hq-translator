add_library(detection text_detector.cpp)
target_include_directories(detection PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(detection ${OpenCV_LIBS})
