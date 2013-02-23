cmake_minimum_required(VERSION 2.6)
project(rb-ogl3)

################################################################################
# Check dependencies
################################################################################
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

################################################################################
# Define target
################################################################################
file(GLOB RBOGL3_FILES *.c)
add_library(rb-ogl3 SHARED ${RBOGL3_FILES})

target_link_libraries(rb-ogl3 ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${SNLSYS_LIBRARY})
set_target_properties(rb-ogl3 PROPERTIES DEFINE_SYMBOL RB_SHARED_BUILD)

################################################################################
# Define outputs
################################################################################
install(TARGETS rb-ogl3 LIBRARY DESTINATION lib)

