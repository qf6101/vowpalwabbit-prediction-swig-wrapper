# include head files and link libraries
include_directories(${PROJECT_SOURCE_DIR}/core/include)
include_directories(${PROJECT_SOURCE_DIR}/externals/include)
link_directories(${PROJECT_SOURCE_DIR}/externals/lib/gtest)
# add test executable
file(GLOB src_files "src/*/*.cpp")
set(SOURCE_FILES ${src_files} src/main.cpp)
add_executable(pctr_test ${SOURCE_FILES})
target_link_libraries(pctr_test
        externals
        pctr_core
        gtest
        gtest_main
        boost_thread
        boost_system
        pthread
        boost_filesystem)