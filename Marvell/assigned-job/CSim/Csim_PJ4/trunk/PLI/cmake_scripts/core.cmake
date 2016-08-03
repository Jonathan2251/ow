add_custom_target (CREATE_INTERFACE_CPP ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/__interface_header.h)
add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/__interface_header.h
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/package/interface_v8
    COMMAND ${PYTHON_EXE} gen.py ${PROJECT_SOURCE_DIR}/include/rtl_interface_functions.h > interface.cpp
    )
add_subdirectory (${PROJECT_SOURCE_DIR}/src/v8/aarch64/system_register ${CMAKE_CURRENT_BINARY_DIR}/obj/v8/aarch64/system_register)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/utility ${CMAKE_CURRENT_BINARY_DIR}/obj/utility)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/coprocessor ${CMAKE_CURRENT_BINARY_DIR}/obj/coprocessor)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/debugarch ${CMAKE_CURRENT_BINARY_DIR}/obj/debugarch)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/execution ${CMAKE_CURRENT_BINARY_DIR}/obj/execution)
add_subdirectory (${PROJECT_SOURCE_DIR}/package/gdb_stub/linux ${CMAKE_CURRENT_BINARY_DIR}/obj/gdb_stub)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/instruction ${CMAKE_CURRENT_BINARY_DIR}/obj/inst)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/memory ${CMAKE_CURRENT_BINARY_DIR}/obj/memory)
add_subdirectory (${PROJECT_SOURCE_DIR}/package/model ${CMAKE_CURRENT_BINARY_DIR}/obj/package/model)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/mpcore ${CMAKE_CURRENT_BINARY_DIR}/obj/mpcore)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/net_console ${CMAKE_CURRENT_BINARY_DIR}/obj/net_console)
add_subdirectory (${PROJECT_SOURCE_DIR}/lib/SoftFloat-2b/softfloat/bits32 ${CMAKE_CURRENT_BINARY_DIR}/obj/lib/softfloat)

add_dependencies (utility CREATE_INTERFACE_CPP)
