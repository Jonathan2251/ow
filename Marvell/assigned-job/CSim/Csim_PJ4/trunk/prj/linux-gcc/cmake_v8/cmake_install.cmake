# Install script for directory: /home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch64/system_register/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/utility/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch64/execution/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch64/inst/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch64/interprocessing/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch64/memory/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch64/utility/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch32/coprocessor/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch32/execution/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch32/instruction/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/aarch32/memory/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/coprocessor/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/debugarch/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/execution/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/gdb_stub/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/inst/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/interface_v8/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/memory/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/package/model/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/mpcore/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/net_console/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/v8/shared_func/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/TarmacParser/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/lib/softfloat/cmake_install.cmake")
  INCLUDE("/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/obj/lib/softfloat64/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/cschen/test/2/Csim_PJ4/trunk/prj/linux-gcc/cmake_v8/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
