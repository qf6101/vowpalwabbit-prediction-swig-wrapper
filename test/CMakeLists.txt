# include head files
include_directories(${PROJECT_SOURCE_DIR}/core/include)
# add googletest library
add_subdirectory(googletest)
include_directories(googletest/googlemock/include)
include_directories(googletest/googletest/include)
# add test executable
set(SOURCE_FILES
        main.cpp
        test_feature_hashing.cpp
        test_linear_model.cpp
        test_model_daemon.cpp
        test_configuration.cpp)
add_executable(pctr_test ${SOURCE_FILES})
target_link_libraries(pctr_test pctr_core gtest gtest_main)