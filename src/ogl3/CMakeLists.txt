# Copyright (c) 2013 Vincent Forest
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
cmake_minimum_required(VERSION 2.6)
project(rb-ogl3)

if(NOT CMAKE_COMPILER_IS_GNUCC)
  message(FATAL_ERROR "Unsupported compiler")
endif(NOT CMAKE_COMPILER_IS_GNUCC)

set(CMAKE_C_FLAGS "-pedantic -std=c99 -Wall -Wextra -Wcast-align -Wmissing-declarations -Wmissing-prototypes -fvisibility=hidden -fstrict-aliasing -Wl,-z,defs")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

################################################################################
# Check dependencies
################################################################################
find_path(SYS_INCLUDE_DIR "sys/sys.h")
find_path(GL_INCLUDE_DIR  NAMES GL/gl.h GL/glext.h)
find_path(GLU_INCLUDE_DIR  NAMES GL/glu.h)
find_library(SYS_LIBRARY NAMES sys)
find_library(GL_LIBRARY NAMES GL)
find_library(GLU_LIBRARY NAMES GLU)

if(NOT SYS_INCLUDE_DIR OR NOT SYS_LIBRARY)
  message(FATAL_ERROR "Can't find sys header/library")
elseif(NOT GL_INCLUDE_DIR OR NOT GL_LIBRARY)
  message(FATAL_ERROR "Can't find GL headers/library")
elseif(NOT GLU_INCLUDE_DIR OR NOT GLU_LIBRARY)
  message(FATAL_ERROR "Can't find GLU header/library")
else()
  message(STATUS "sys found: ${SYS_LIBRARY}")
  message(STATUS "GL found: ${GL_LIBRARY}")
  message(STATUS "GLU found: ${GLU_LIBRARY}")
endif()

include_directories(${GL_INCLUDE_DIR})
include_directories(${SYS_INCLUDE_DIR})

################################################################################
# Define target
################################################################################
file(GLOB RBOGL3_FILES *.c)
add_library(rb-ogl3 SHARED ${RBOGL3_FILES})

target_link_libraries(rb-ogl3 ${GL_LIBRARY} ${GLU_LIBRARY} ${SYS_LIBRARY})
set_target_properties(rb-ogl3 PROPERTIES DEFINE_SYMBOL RB_SHARED_BUILD)
# FIXME remove this flag
#set_target_properties(rb-ogl3 PROPERTIES COMPILE_DEFINITIONS
#  GL_GLEXT_PROTOTYPES)

################################################################################
# Define outputs
################################################################################
install(TARGETS rb-ogl3 LIBRARY DESTINATION lib)

