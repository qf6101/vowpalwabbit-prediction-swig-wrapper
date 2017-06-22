//
// Created by qfeng on 17-5-31.
//

#include <gtest/gtest.h>
#include "model/model_daemon.h"
#include <boost/function.hpp>
#include <model/linear_model.h>
#include <spdlog/spdlog.h>
#include <log/pctr_log.h>


using std::string;

namespace ctr_test {

    class test_model_daemon : public ::testing::Test {
    protected:
        unique_ptr<model_daemon> _daemon;
        shared_ptr<spdlog::logger> _logger;

        test_model_daemon() {
            _logger = spdlog::get(pctr_log::_core_logger_name);
            _daemon.reset(new model_daemon("test/data/vw_model.txt", "linear_model", "test/data/word2vec_model.txt", 1));
        }

        ~test_model_daemon() {
        }

    public:
        void thread_content() {
            bool continue_loop = true;
            while(continue_loop) {
                _logger->debug("test_model_daemon: pass one loop in thread content...");
                boost::this_thread::sleep(boost::posix_time::seconds(10));
            }
        }
    };

    TEST_F(test_model_daemon, load_model) {
        auto out_linear_model = dynamic_pointer_cast<linear_model>(_daemon->load_model());
        EXPECT_EQ(out_linear_model->weight_size(), pow(2, 18));
        EXPECT_NEAR(out_linear_model->get(11), -0.00402038, 1E-8);
        EXPECT_NEAR(out_linear_model->get(169320), 0.0352785, 1E-8);
        EXPECT_NEAR(out_linear_model->get(262110), 0.0423157, 1E-8);
    }

    TEST_F(test_model_daemon, is_model_file_modified) {
        bool first_touch = _daemon->is_model_file_modified();
        EXPECT_EQ(first_touch, true);
        auto out_model = _daemon->load_model();
        bool second_touch = _daemon->is_model_file_modified();
        EXPECT_EQ(second_touch, false);
    }

    TEST_F(test_model_daemon, boost_thread) {
        boost::function0<void> f = boost::bind(&test_model_daemon::thread_content, this);
        boost::thread thrd(f);
        _logger->debug("test_model_daemon: sleep 2 seconds starts...");
        boost::this_thread::sleep(boost::posix_time::seconds(2));
        _logger->debug("test_model_daemon: sleep 2 seconds ends...");
        thrd.interrupt();
        thrd.join();
        _logger->debug("test_model_daemon: terminate thread!");
    }

} //namespace