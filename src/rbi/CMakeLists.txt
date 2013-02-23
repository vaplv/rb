cmake_minimum_required(VERSION 2.6)
project(rbi C)

file(GLOB RBI_FILES *.c)
add_library(rbi SHARED ${RBI_FILES})
target_link_libraries(rbi dl)
set_target_properties(rbi PROPERTIES DEFINE_SYMBOL RBI_SHARED_BUILD)

install(TARGETS rbi LIBRARY DESTINATION lib)
install(FILES rbi.h DESTINATION include/rb)
