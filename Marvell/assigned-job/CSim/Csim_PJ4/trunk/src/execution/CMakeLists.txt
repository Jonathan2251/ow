include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../../lib/SoftFloat-2b/softfloat/bits32/386-Win32-GCC")
file (GLOB_RECURSE SRC_FILES *.cpp)
add_library (execution ${SRC_FILES})
