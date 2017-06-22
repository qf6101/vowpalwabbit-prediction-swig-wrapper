# initialize project
cmake_minimum_required(VERSION 3.9)
project(PCTR LANGUAGES VERSION 1.0.0 C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# add DEBUG flag
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
# add ctest support
include(CTest)
# add global includes and libraries (e.g., boost and Eigen)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
add_definitions(-DBOOST_LOG_DYN_LINK=1)
# add sub projects
add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(swig)
add_subdirectory(externals)