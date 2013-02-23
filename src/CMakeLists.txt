cmake_minimum_required(VERSION 2.6)
project(rb C)

################################################################################
# Setup compiler
################################################################################
if(NOT CMAKE_COMPILER_IS_GNUCC)
  message(FATAL_ERROR "Unsupported compiler")
endif(NOT CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_DEBUG_POSTFIX "-dbg")

set(CMAKE_C_FLAGS "-pedantic -std=c99 -Wall -Wextra -Wcast-align -Wmissing-declarations -Wmissing-prototypes -fvisibility=hidden -fstrict-aliasing -Wl,-z,defs")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

################################################################################
# Check dependencies
################################################################################
find_path(SNLSYS_INCLUDE_DIR "snlsys/snlsys.h")
find_library(SNLSYS_LIBRARY NAMES snlsys)

if(NOT SNLSYS_INCLUDE_DIR OR NOT SNLSYS_LIBRARY)
  message(FATAL_ERROR "Can't find snlsys headers/library")
else()
  message(STATUS "snlsys found: ${SNLSYS_LIBRARY}")
endif()

include_directories(${SNLSYS_INCLUDE_DIR} ./)

################################################################################
# Output files
################################################################################
file(GLOB RB_FILES_INC *.h)
install(FILES ${RB_FILES_INC} DESTINATION include/rb)

################################################################################
# Sub projects
################################################################################
add_subdirectory(example)
add_subdirectory(null)
add_subdirectory(ogl3)
add_subdirectory(rbi)

