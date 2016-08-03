if (${BUILD_EXE} STREQUAL CURRENT)
  add_executable (csim_v8 ${PROJECT_SOURCE_DIR}/src/main/main_v8.cpp)
  target_link_libraries (csim_v8 ${AARCH64_INST_OBJECTS} ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_v8 ${CSIM_NEEDED_LIBS} csim)

  add_executable (csim_v8_mpcore ${PROJECT_SOURCE_DIR}/src/main/main_v8_mpcore.cpp)
  target_link_libraries (csim_v8_mpcore ${AARCH64_INST_OBJECTS} ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_v8_mpcore ${CSIM_NEEDED_LIBS})

  add_executable (debuglevel ${PROJECT_SOURCE_DIR}/src/main/debuglevel.cpp)
  target_link_libraries (debuglevel ${AARCH64_INST_OBJECTS} ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (debuglevel ${CSIM_NEEDED_LIBS})

  add_executable (net_console_exe ${PROJECT_SOURCE_DIR}/src/main/net_console.cpp)
  target_link_libraries (net_console_exe ${AARCH64_INST_OBJECTS} ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (net_console_exe ${CSIM_NEEDED_LIBS})

  add_executable (csim_mpcore ${PROJECT_SOURCE_DIR}/src/main/main_mpcore.cpp)
  target_link_libraries (csim_mpcore ${AARCH64_INST_OBJECTS} ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_mpcore ${CSIM_NEEDED_LIBS})

  add_executable (csim_dpi ${PROJECT_SOURCE_DIR}/src/main/csim_dpi.cpp)
  target_link_libraries (csim_dpi ${AARCH64_INST_OBJECTS} ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_dpi ${CSIM_NEEDED_LIBS})

  add_executable (cfgquery ${PROJECT_SOURCE_DIR}/src/main/cfgmgr_main.cpp)
  target_link_libraries (cfgquery ${AARCH64_INST_OBJECTS} ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (cfgquery ${CSIM_NEEDED_LIBS})
elseif (${BUILD_EXE} STREQUAL "OBJECT")
  add_executable (csim_v8 ${PROJECT_SOURCE_DIR}/src/main/main_v8.cpp $<TARGET_OBJECTS:AARCH64_INST_OBJECTS>)
  target_link_libraries (csim_v8 ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_v8 ${CSIM_NEEDED_LIBS} csim)

  add_executable (csim_v8_mpcore ${PROJECT_SOURCE_DIR}/src/main/main_v8_mpcore.cpp $<TARGET_OBJECTS:AARCH64_INST_OBJECTS>)
  target_link_libraries (csim_v8_mpcore ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_v8_mpcore ${CSIM_NEEDED_LIBS})

  add_executable (debuglevel ${PROJECT_SOURCE_DIR}/src/main/debuglevel.cpp $<TARGET_OBJECTS:AARCH64_INST_OBJECTS>)
  target_link_libraries (debuglevel ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (debuglevel ${CSIM_NEEDED_LIBS})

  add_executable (net_console_exe ${PROJECT_SOURCE_DIR}/src/main/net_console.cpp $<TARGET_OBJECTS:AARCH64_INST_OBJECTS>)
  target_link_libraries (net_console_exe ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (net_console_exe ${CSIM_NEEDED_LIBS})

  add_executable (csim_mpcore ${PROJECT_SOURCE_DIR}/src/main/main_mpcore.cpp $<TARGET_OBJECTS:AARCH64_INST_OBJECTS>)
  target_link_libraries (csim_mpcore ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_mpcore ${CSIM_NEEDED_LIBS})

  add_executable (csim_dpi ${PROJECT_SOURCE_DIR}/src/main/csim_dpi.cpp $<TARGET_OBJECTS:AARCH64_INST_OBJECTS>)
  target_link_libraries (csim_dpi ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_dpi ${CSIM_NEEDED_LIBS})

  add_executable (cfgquery ${PROJECT_SOURCE_DIR}/src/main/cfgmgr_main.cpp $<TARGET_OBJECTS:AARCH64_INST_OBJECTS>)
  target_link_libraries (cfgquery ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (cfgquery ${CSIM_NEEDED_LIBS})
elseif (${BUILD_EXE} STREQUAL "CPP")
  add_executable (csim_v8 ${PROJECT_SOURCE_DIR}/src/main/main_v8.cpp ${AARCH64_INST_CPP})
  target_link_libraries (csim_v8 ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_v8 ${CSIM_NEEDED_LIBS} csim)

  add_executable (csim_v8_mpcore ${PROJECT_SOURCE_DIR}/src/main/main_v8_mpcore.cpp ${AARCH64_INST_CPP})
  target_link_libraries (csim_v8_mpcore ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_v8_mpcore ${CSIM_NEEDED_LIBS})

  add_executable (debuglevel ${PROJECT_SOURCE_DIR}/src/main/debuglevel.cpp ${AARCH64_INST_CPP})
  target_link_libraries (debuglevel ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (debuglevel ${CSIM_NEEDED_LIBS})

  add_executable (net_console_exe ${PROJECT_SOURCE_DIR}/src/main/net_console.cpp ${AARCH64_INST_CPP})
  target_link_libraries (net_console_exe ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (net_console_exe ${CSIM_NEEDED_LIBS})

  add_executable (csim_mpcore ${PROJECT_SOURCE_DIR}/src/main/main_mpcore.cpp ${AARCH64_INST_CPP})
  target_link_libraries (csim_mpcore ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_mpcore ${CSIM_NEEDED_LIBS})

  add_executable (csim_dpi ${PROJECT_SOURCE_DIR}/src/main/csim_dpi.cpp ${AARCH64_INST_CPP})
  target_link_libraries (csim_dpi ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (csim_dpi ${CSIM_NEEDED_LIBS})

  add_executable (cfgquery ${PROJECT_SOURCE_DIR}/src/main/cfgmgr_main.cpp ${AARCH64_INST_CPP})
  target_link_libraries (cfgquery ${CSIM_NEEDED_LIBS} ${CSIM_ZLIB})
  add_dependencies (cfgquery ${CSIM_NEEDED_LIBS})
else (${BUILD_EXE} STREQUAL "CURRENT")
  message("BUILD_EXE's option is wrong!!!")
endif (${BUILD_EXE} STREQUAL CURRENT)
