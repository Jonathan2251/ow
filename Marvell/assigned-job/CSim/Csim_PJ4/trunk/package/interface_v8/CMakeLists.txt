execute_process(
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/package/interface_v8/
  COMMAND ${PYTHON_EXE} gen.py ${PROJECT_SOURCE_DIR}/include/rtl_interface_functions.h OUTPUT_FILE interface.cpp
)

file (GLOB_RECURSE SRC_FILES *.cpp)
add_library (interface_v8 ${SRC_FILES})
