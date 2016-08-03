set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../../)
get_filename_component (PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR} ABSOLUTE)
set (INCDIR "${PROJECT_SOURCE_DIR}/include")
include_directories ("${PROJECT_SOURCE_DIR}/lib/hashtable")
include_directories ("${INCDIR}")
include_directories ("${PROJECT_SOURCE_DIR}/lib/zlib-1.2.8/include")

set (CMAKE_CXX_COMPILER "g++")
set (CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER})
set (PYTHON_EXE "python")
set (WARNFLAGS "-Wall -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wshadow -Wcast-qual -Wconversion -Weffc++ -Wextra -Winit-self -Wcast-align")
set (DEF "-DLINUX -DMOLTRES_CORE")
set (CMAKE_CXX_FLAGS "${USERDEF} ${DEF} -fPIC -O2 ${WARNFLAGS}")
set (CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
set (CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> rcs <TARGET> <LINK_FLAGS> <OBJECTS>")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../exe)
set (CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER>  ${DEF} <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> -Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group")

add_definitions (-DADD_SYS_L2C)
add_definitions (-DEVEREST)
add_definitions (-DFUSEDMAC)
add_definitions (-DLPAE)
add_definitions (-DADD_NEON)
add_definitions (-DVFPV3_HALF_EXTENSION)

function(GetObjects CPP_FILES PARENT_PATH REPLACE_PATH)
    get_filename_component (PARENT_PATH ${PARENT_PATH} ABSOLUTE)
    get_filename_component (REPLACE_PATH ${REPLACE_PATH} ABSOLUTE)
    set (OBJ_LISTS)
    foreach (CPP_FILE ${${CPP_FILES}})
        get_filename_component (CPP_FILE ${CPP_FILE} ABSOLUTE)
        file (RELATIVE_PATH CPP_FILE ${PARENT_PATH} ${CPP_FILE}) 
        list (APPEND OBJ_LISTS ${REPLACE_PATH}/${CPP_FILE}.o)
    endforeach ()
    set (${CPP_FILES} ${OBJ_LISTS} PARENT_SCOPE)
endfunction ()

set (CSIM_ZLIB ${PROJECT_SOURCE_DIR}/lib/zlib-1.2.8/libz.so)
add_custom_target (CSIM_ZLIB ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/__header.h)
add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/__header.h
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/zlib-1.2.8
    COMMAND chmod a+x configure
    COMMAND ./configure --prefix=./
    COMMAND make install
    COMMAND rm -rf lib/libz.so*
    )

