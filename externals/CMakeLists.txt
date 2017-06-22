# include head files
include_directories(include)
# construct source files
file(GLOB src_files "src/*/*.cpp")
set(SOURCE_FILES ${src_files})
# add source files to this dynamic library
add_library(externals SHARED ${SOURCE_FILES})