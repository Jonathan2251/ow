cmake_minimum_required (VERSION 2.6)
project (CSIM)

option(TRACE "Tracing varaibles" OFF)

set (CSIM_NEEDED_LIBS aa64execution aa64inst aa64interprocessing aa64memory aa64system_register aa64utility execution memory coprocessor inst utility mpcore net_console debugarch gdb_stub interface_v8 model softfloat softfloat64 tarmacparser aa32execution aa32memory aa32coprocessor aa32instruction sharedfunc)

option(USE_SYSTEM_ZLIB "ON: Use system zlib, OFF: Use Csim source zlib which come from open source code" ON)
if (USE_SYSTEM_ZLIB)
    set (CSIM_ZLIB z)
else (USE_SYSTEM_ZLIB)
    include_directories ("${PROJECT_SOURCE_DIR}/../../../lib/zlib-1.2.8/include")
    set (CSIM_ZLIB ${CMAKE_CURRENT_BINARY_DIR}/lib/zlib-1.2.8/libz.so)
    set (CSIM_ZLIB_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib/zlib-1.2.8)
    add_subdirectory (${PROJECT_SOURCE_DIR}/../../../lib/zlib-1.2.8 ${CMAKE_CURRENT_BINARY_DIR}/lib/zlib-1.2.8)
    #The following add_custom_command needed on sun grid while it is needless on my linux PC
    add_custom_command (
        OUTPUT ${CSIM_ZLIB}
        COMMAND rm -f libz.so
        COMMAND ln -s libz.so.1.2.8 libz.so
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/zlib-1.2.8
        DEPENDS zlib
        )
endif (USE_SYSTEM_ZLIB)

include (${CMAKE_CURRENT_BINARY_DIR}/environment.cmake NO_POLICY_SCOPE)
include (${CMAKE_CURRENT_BINARY_DIR}/core.cmake NO_POLICY_SCOPE)

add_dependencies (utility ${CSIM_ZLIB})

file (GLOB_RECURSE INTERFACE_V8_OBJECTS ${PROJECT_SOURCE_DIR}/package/interface_v8/*.cpp)
file (GLOB_RECURSE AARCH64_INST_OBJECTS ${PROJECT_SOURCE_DIR}/src/v8/aarch64/instruction/*.cpp)

GetObjects(INTERFACE_V8_OBJECTS ${PROJECT_SOURCE_DIR}/package/interface_v8 obj/interface_v8/CMakeFiles/interface_v8.dir)
GetObjects(AARCH64_INST_OBJECTS ${PROJECT_SOURCE_DIR}/src/v8/aarch64/instruction obj/v8/aarch64/inst/CMakeFiles/aa64inst.dir)

if (TRACE)
    message("USE_SYSTEM_ZLIB=${USE_SYSTEM_ZLIB}")
    message("INTERFACE_V8_OBJECTS=${INTERFACE_V8_OBJECTS}")
endif(TRACE)

if (USE_SYSTEM_ZLIB)
    add_custom_command (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../libcsim.so
        COMMAND ${CMAKE_CXX_COMPILER} -shared -lz -export-dynamic -Wl,-noinhibit-exec -fPIC -o ${CMAKE_CURRENT_BINARY_DIR}/../libcsim.so -Wl,--start-group ${INTERFACE_V8_OBJECTS} ${AARCH64_INST_OBJECTS} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/*.a -Wl,--end-group
        DEPENDS ${CSIM_NEEDED_LIBS}
    )
else (USE_SYSTEM_ZLIB)
    add_custom_command (
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../libcsim.so
        COMMAND ${CMAKE_CXX_COMPILER} -shared -L${CSIM_ZLIB_PATH} -lz -export-dynamic -Wl,-noinhibit-exec -fPIC -o ${CMAKE_CURRENT_BINARY_DIR}/../libcsim.so -Wl,--start-group ${INTERFACE_V8_OBJECTS} ${AARCH64_INST_OBJECTS} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/*.a -Wl,--end-group
        DEPENDS ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB}
    )
endif (USE_SYSTEM_ZLIB)
add_custom_target (gen_csim_so DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../libcsim.so)

include (${CMAKE_CURRENT_BINARY_DIR}/executable_build.cmake NO_POLICY_SCOPE)

