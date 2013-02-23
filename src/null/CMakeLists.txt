cmake_minimum_required(VERSION 2.6)
project(rb-null C)

file(GLOB RBNULL_FILES *.c)
add_library(rb-null SHARED ${RBNULL_FILES})
set_target_properties(rb-null PROPERTIES DEFINE_SYMBOL RB_SHARED_BUILD)

install(TARGETS rb-null LIBRARY DESTINATION lib)

