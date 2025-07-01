add_library(overlayer overlay_renderer.cpp)
target_include_directories(overlayer PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})
target_link_libraries(overlayer ${OpenCV_LIBS})
