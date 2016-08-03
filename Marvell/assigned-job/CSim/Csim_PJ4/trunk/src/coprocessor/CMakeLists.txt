include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
file (GLOB_RECURSE SRC_FILES *.cpp)
add_library (coprocessor ${SRC_FILES})
