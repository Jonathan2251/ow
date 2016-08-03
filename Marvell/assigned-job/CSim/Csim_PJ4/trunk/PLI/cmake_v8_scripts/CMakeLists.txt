cmake_minimum_required (VERSION 2.6)
project (CSIM)

set (CSIM_NEEDED_LIBS aa64execution aa64inst aa64interprocessing aa64memory aa64system_register aa64utility execution memory coprocessor inst utility mpcore net_console debugarch gdb_stub interface_v8 model softfloat softfloat64 tarmacparser aa32execution aa32memory aa32coprocessor aa32instruction sharedfunc)

include (${CMAKE_CURRENT_BINARY_DIR}/environment.cmake NO_POLICY_SCOPE)
include (${CMAKE_CURRENT_BINARY_DIR}/core.cmake NO_POLICY_SCOPE)

file (GLOB_RECURSE AARCH64_INST_OBJECTS ${PROJECT_SOURCE_DIR}/src/v8/aarch64/instruction/*.cpp)

GetObjects(AARCH64_INST_OBJECTS ${PROJECT_SOURCE_DIR}/src/v8/aarch64/instruction obj/v8/aarch64/inst/CMakeFiles/aa64inst.dir)

add_subdirectory (${PROJECT_SOURCE_DIR}/DPI/v8_src ${CMAKE_CURRENT_BINARY_DIR}/obj/dpi)

file (GLOB_RECURSE DPI_OBJECTS ${PROJECT_SOURCE_DIR}/DPI/v8_src/*.cpp)
GetObjects(DPI_OBJECTS ${PROJECT_SOURCE_DIR}/DPI/v8_src obj/dpi/CMakeFiles/dpi.dir)

set (DEF "${ENV32DEF}")

set (ARCH_LIBS)
foreach (LIB_FILE ${CSIM_NEEDED_LIBS})
    list (APPEND ARCH_LIBS ${CSIM_WORK_BASE}/lib/lib${LIB_FILE}.a)
endforeach ()
list (APPEND ARCH_LIBS ${CSIM_WORK_BASE}/lib/libz.a)

add_custom_target (SHARED_CSIM_LIB ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/share_lib_header.h)
add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/share_lib_header.h
    COMMAND ${CMAKE_CXX_COMPILER} -lz -shared ${DEF} -fPIC -export-dynamic -Wl,-noinhibit-exec -Wl,--start-group -o ${CSIM_WORK_BASE}/${SHARE_LIB_NAME} ${DPI_OBJECTS} ${AARCH64_INST_OBJECTS} ${ARCH_LIBS} -Wl,--end-group
    )
add_dependencies (SHARED_CSIM_LIB ${CSIM_NEEDED_LIBS})
