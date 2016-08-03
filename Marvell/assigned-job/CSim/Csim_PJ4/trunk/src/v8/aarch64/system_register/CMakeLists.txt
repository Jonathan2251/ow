include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib/SoftFloat-2b/softfloat/bits32/386-Win32-GCC")

file (GLOB_RECURSE SYSREG_DEF_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*_aarch64_system_reg.def)

set(ARG1 "${SYSREG_DEF_FILES}")
string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" ARG1 "${ARG1}")
string(REPLACE ";" "," ARG1 "${ARG1}")

string(REPLACE ".def" ".h" SYSREG_GEN_H_FILES "${SYSREG_DEF_FILES}")

add_custom_command (
    OUTPUT ${SYSREG_GEN_H_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/sysreg_name.def ${CMAKE_CURRENT_SOURCE_DIR}/system_register_index.h
# Call by command:
# python def_create.py -i_field=sysreg_field.def -i_arch=fastmodel_aarch64_system_reg.def,everest_aarch64_system_reg.def,apache_aarch64_system_reg.def,dante_aarch64_system_reg.def,ali_aarch64_system_reg.def -o_def=sysreg_name.def -o_h=system_register_index.h
    COMMAND ${PYTHON_EXE} def_create.py -i_field=sysreg_field.def -i_arch=${ARG1} -o_def=sysreg_name.def -o_h=system_register_index.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${SYSREG_DEF_FILES} def_create.py
    )
add_custom_target (def_create DEPENDS ${SYSREG_GEN_H_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/sysreg_name.def ${CMAKE_CURRENT_SOURCE_DIR}/system_register_index.h)

if (TRACE)
  message ("SYSREG_DEF_FILES = ${SYSREG_DEF_FILES}")
  message ("ARG1 = ${ARG1}")
  message ("SYSREG_GEN_H_FILES = ${SYSREG_GEN_H_FILES}")
endif(TRACE)

set (SYS_REG_PERMISSION_DEF_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/permission.h)
set (SYS_REG_PERMISSION_DEF_CPP ${CMAKE_CURRENT_SOURCE_DIR}/permission.cpp)
add_custom_command (
    OUTPUT ${SYS_REG_PERMISSION_DEF_HEADER} ${SYS_REG_PERMISSION_DEF_CPP}
    COMMAND ${PYTHON_EXE} permission_create.py -i=permission.def -o_h=permission.h -o_cpp=permission.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/permission.def permission_create.py
    )
add_custom_target (permission_create DEPENDS ${SYS_REG_PERMISSION_DEF_HEADER} ${SYS_REG_PERMISSION_DEF_CPP})

file (GLOB_RECURSE SRC_FILES *.cpp)
set (SRC_FILES ${SRC_FILES} ${SYS_REG_PERMISSION_DEF_CPP})
list (REMOVE_DUPLICATES SRC_FILES)
add_library (aa64system_register ${SRC_FILES})
add_dependencies (aa64system_register def_create permission_create)
