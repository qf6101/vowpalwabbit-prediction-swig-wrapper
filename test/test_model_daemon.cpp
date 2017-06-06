//
// Created by qfeng on 17-5-31.
//

#include <gtest/gtest.h>
#include <config/configuration.h>
#include "model/model_daemon.h"
#include <boost/function.hpp>
#include <model/linear_model.h>


using std::string;

namespace ctr_test {

    class test_model_daemon : public ::testing::Test {
    protected:
        model_daemon* daemon = NULL;

        virtual void SetUp() {
            init_log();
            daemon = new model_daemon("test/data/vw_model.txt", "linear_model");
            daemon->_update_interval = 1;
        }

        virtual void TearDown() {
            if (daemon != NULL) delete daemon;
        }

    public:
        void thread_content() {
            bool continue_loop = true;
            while(continue_loop) {
                BOOST_LOG_TRIVIAL(debug) << "pass one loop in thread content...";
                boost::this_thread::sleep(boost::posix_time::seconds(10));
            }
        }
    };

    TEST_F(test_model_daemon, load_model) {
        model *out_model = NULL;
        daemon->load_model(&out_model);
        linear_model* out_linear_model = dynamic_cast<linear_model*>(out_model);
        EXPECT_EQ(out_linear_model->weight_size(), pow(2, 18));
        EXPECT_NEAR(out_linear_model->get(11), -0.00402038, 1E-8);
        EXPECT_NEAR(out_linear_model->get(169320), 0.0352785, 1E-8);
        EXPECT_NEAR(out_linear_model->get(262110), 0.0423157, 1E-8);
        if (out_linear_model != NULL) delete out_model;
    }

    TEST_F(test_model_daemon, is_model_file_modified) {
        bool first_touch = daemon->is_model_file_modified();
        EXPECT_EQ(first_touch, true);
        model *out_model = NULL;
        daemon->load_model(&out_model);
        bool second_touch = daemon->is_model_file_modified();
        EXPECT_EQ(second_touch, false);
        if (out_model != NULL) delete out_model;
    }

    TEST_F(test_model_daemon, boost_thread) {
        boost::function0<void> f = boost::bind(&test_model_daemon::thread_content, this);
        boost::thread thrd(f);
        BOOST_LOG_TRIVIAL(debug) << "sleep 2 seconds starts...";
        boost::this_thread::sleep(boost::posix_time::seconds(2));
        BOOST_LOG_TRIVIAL(debug) << "sleep 2 seconds ends...";
        thrd.interrupt();
        thrd.join();
        BOOST_LOG_TRIVIAL(debug) << "terminate thread!";
    }

} //namespace