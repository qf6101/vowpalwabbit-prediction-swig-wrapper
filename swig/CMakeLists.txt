# add swig and java supports
find_package(SWIG REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)
include(UseSWIG)
include(${SWIG_USE_FILE})
include_directories(${JNI_INCLUDE_DIRS})
set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.8" "-target" "1.8")
enable_testing()
# configure swig properties (avoiding using -includeall)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_SWIG_FLAGS -package io.github.qf6101.pctrswig)
set_source_files_properties(pctr.i PROPERTIES CPLUSPLUS ON)
# add head files
include_directories(${PROJECT_SOURCE_DIR}/core/include)
include_directories(${PROJECT_SOURCE_DIR}/externals/include)
# add swig files
swig_add_library(
        pctr_swig
        TYPE SHARED
        LANGUAGE java
        SOURCES pctr.i
)
# link to core sub projects
SWIG_LINK_LIBRARIES(pctr_swig pctr_core)
# extract java files and c++ lib files
add_custom_command(TARGET pctr_swig POST_BUILD
        COMMAND rm -rf ${PROJECT_BINARY_DIR}/swig/lib
        COMMAND mkdir ${PROJECT_BINARY_DIR}/swig/lib
        COMMAND cp ${PROJECT_BINARY_DIR}/swig/*.so ${PROJECT_BINARY_DIR}/swig/lib/
        COMMAND rm -rf ${PROJECT_BINARY_DIR}/swig/java
        COMMAND mkdir ${PROJECT_BINARY_DIR}/swig/java
        COMMAND cp ${PROJECT_BINARY_DIR}/swig/*.java ${PROJECT_BINARY_DIR}/swig/java/
        # create log directory for spdlog
        COMMAND rm -rf ${PROJECT_BINARY_DIR}/swig/pctr_logs/swig_test
        COMMAND mkdir -p ${PROJECT_BINARY_DIR}/swig/pctr_logs/swig_test)
# prepare the test of generated java files and c++ dynamic library
file(GLOB java_src ${PROJECT_BINARY_DIR}/swig/java/*.java)
add_jar(pctr_swig_test ${java_src} ${PROJECT_SOURCE_DIR}/swig/pctr_swig_test.java)
get_target_property(_jarFile pctr_swig_test JAR_FILE)
get_target_property(_classDir pctr_swig_test CLASSDIR)
message(STATUS "Jar file ${_jarFile}")
message(STATUS "Class compiled to ${_classDir}")
# test invoking swig generated files
add_test(NAME PctrSwigTest COMMAND ${Java_JAVA_EXECUTABLE} -cp ${_jarFile} -Djava.library.path=${PROJECT_BINARY_DIR}/swig/lib io.github.qf6101.pctrswigtest.PctrSwigTest)
set_tests_properties(PctrSwigTest PROPERTIES PASS_REGULAR_EXPRESSION "98765")

