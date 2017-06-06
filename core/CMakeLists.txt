# include head files
include_directories(include)
# construct source files
file(GLOB src_files "src/*/*")
set(SOURCE_FILES ${src_files})
# add source files to this dynamic library
add_library(pctr_core SHARED ${SOURCE_FILES})
target_link_libraries(pctr_core boost_thread boost_system boost_log boost_log_setup)