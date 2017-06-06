//
// Created by qfeng on 17-5-31.
//

#include <gtest/gtest.h>
#include "config/configuration.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace ctr_test {

    class test_configuration : public ::testing::Test {
    protected:
        virtual void SetUp() {
            init_log();
        }
    };

    TEST_F(test_configuration, boost_log_trivial) {
        BOOST_LOG_TRIVIAL(trace) <<  "test trace";
        BOOST_LOG_TRIVIAL(debug) << "test debug";
        BOOST_LOG_TRIVIAL(info) << "test info";
        BOOST_LOG_TRIVIAL(warning) << "test warning";
        BOOST_LOG_TRIVIAL(error) << "test error";
        BOOST_LOG_TRIVIAL(fatal) << "test fatal";
    }


} //namespace