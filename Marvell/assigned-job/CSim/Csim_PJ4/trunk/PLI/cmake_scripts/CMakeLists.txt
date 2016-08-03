cmake_minimum_required (VERSION 2.6)
project (CSIM)

set (CSIM_NEEDED_LIBS aa64system_register utility coprocessor debugarch execution gdb_stub inst memory model mpcore net_console softfloat dpi)

include (${CMAKE_CURRENT_BINARY_DIR}/environment.cmake NO_POLICY_SCOPE)
include (${CMAKE_CURRENT_BINARY_DIR}/core.cmake NO_POLICY_SCOPE)

add_subdirectory (${PROJECT_SOURCE_DIR}/DPI/src ${CMAKE_CURRENT_BINARY_DIR}/obj/dpi)

file (GLOB_RECURSE DPI_OBJECTS ${PROJECT_SOURCE_DIR}/DPI/src/*.cpp)
GetObjects(DPI_OBJECTS ${PROJECT_SOURCE_DIR}/DPI/src obj/dpi/CMakeFiles/dpi.dir)

set (DEF "-m32")

set (ARCH_LIBS)
foreach (LIB_FILE ${CSIM_NEEDED_LIBS})
    list (APPEND ARCH_LIBS ${CSIM_WORK_BASE}/lib/lib${LIB_FILE}.a)
endforeach ()
list (APPEND ARCH_LIBS ${CSIM_WORK_BASE}/lib/libz.a)

add_custom_target (SHARED_CSIM_LIB ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/share_lib_header.h)
add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/share_lib_header.h
    COMMAND ${CMAKE_CXX_COMPILER} -shared -export-dynamic -Wl,-noinhibit-exec -Wl,--start-group ${DEF} -o ${CSIM_WORK_BASE}/dpi_${SIM_TYPE}.so ${DPI_OBJECTS} ${ARCH_LIBS} -Wl,--end-group
    )
add_dependencies (SHARED_CSIM_LIB ${CSIM_NEEDED_LIBS})
