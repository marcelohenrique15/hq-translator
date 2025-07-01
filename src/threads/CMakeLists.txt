add_library(threads
    webcam_thread.cpp
)

target_include_directories(threads PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
