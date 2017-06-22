# include head files
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/externals/include)
# construct source files
file(GLOB src_files "src/*/*.cpp")
set(SOURCE_FILES ${src_files})
# add source files to this dynamic library
add_library(pctr_core SHARED ${SOURCE_FILES})
target_link_libraries(pctr_core
        externals
        boost_thread
        pthread
        boost_system)