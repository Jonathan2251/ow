cmake_minimum_required(VERSION 2.6)

#set (CMAKE_VERBOSE_MAKEFILE true)

set (SRC_PATH ../../../../../src/v8/aarch64)
set (GTEST_ROOT ../../../../../lib/google_test)
set (CSIM_ROOT ../../../../../prj/linux-gcc)

include_directories (${SRC_PATH}/system_register/)
include_directories (${SRC_PATH}/execution/)
include_directories (../../../../../include/)
include_directories (${GTEST_ROOT}/include/)

set (WARNING_FLAGS "-Wall -Wold-style-cast -Woverloaded-virtual -Wfloat-equal -Wshadow -Wcast-qual -Wconversion -Wextra -Winit-self -Wcast-align -Weffc++")
set (CMAKE_CXX_FLAGS "-g ${WARNING_FLAGS}")
set (SYSTEM_REGISTER_PATH ${SRC_PATH}/system_register)
set (STATE_SYSREG_PATH ${SRC_PATH}/system_register/state_reg)
set (EXECUTION_PATH ${SRC_PATH}/execution)
set (CSIM_LIB libcsim_test.a)
set (GTEST_LIB libgtest.a)
set (AARCH64_SYSREG_OBJECTS
         ${SYSTEM_REGISTER_PATH}/sysreg.cpp
         ${SYSTEM_REGISTER_PATH}/ro_sysreg.cpp
         ${SYSTEM_REGISTER_PATH}/esr_sysreg.cpp
         ${SYSTEM_REGISTER_PATH}/system_register_manager.cpp
         ${STATE_SYSREG_PATH}/spsel.cpp
         ${STATE_SYSREG_PATH}/daif.cpp
         ${STATE_SYSREG_PATH}/cur_el.cpp
         ${STATE_SYSREG_PATH}/nzcv.cpp
		 ${EXECUTION_PATH}/processorstate.cpp
     )

add_custom_target(sysmgr_definition_file ALL
	./registeration.py v8_aarch64_system_register_encoding.htm
	WORKING_DIRECTORY ${SRC_PATH}/system_register/
)

link_directories(${CSIM_ROOT})
link_directories(${GTEST_ROOT})

link_libraries(pthread)

add_library(aarch64_sysreg_lib
		${AARCH64_SYSREG_OBJECTS})
add_dependencies(aarch64_sysreg_lib sysmgr_definition_file)

add_executable (sysreg_test sysreg_test.cpp)
target_link_libraries (sysreg_test aarch64_sysreg_lib ${CSIM_LIB} ${GTEST_LIB})
