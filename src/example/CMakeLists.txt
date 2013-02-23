cmake_minimum_required(VERSION 2.6)
project(eg-rb C)

################################################################################
# Check dependencies
################################################################################
find_path(WM_INCLUDE_DIR "wm/wm.h")
find_library(WM_LIBRARY NAMES wm-glfw)

if(NOT WM_INCLUDE_DIR OR NOT WM_LIBRARY)
  message(FATAL_ERROR "Can't find window manager header/library")
else()
  message(STATUS "wm found: ${WM_LIBRARY}")
endif()

include_directories(${WM_INCLUDE_DIR})

################################################################################
# Define target
################################################################################
add_executable(eg_rb eg_rb.c)
target_link_libraries(eg_rb rbi m ${WM_LIBRARY})

