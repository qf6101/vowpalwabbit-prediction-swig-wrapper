//
// Created by qfeng on 17-6-21.
//

#include <gtest/gtest.h>
#include <CTPL/ctpl.h>
#include <boost/filesystem/operations.hpp>
#include "prediction/linear_predictor.h"

namespace bfs = boost::filesystem;

namespace ctr_test {

    class test_linear_predictor : public ::testing::Test {
    protected:

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };


    TEST_F(test_linear_predictor, predict) {
        string model_path = "test/data/vw_model.txt";
        string w2v_model_path = "test/data/word2vec_model.txt";
        auto predictor = unique_ptr<linear_predictor>(new linear_predictor(model_path, w2v_model_path, 1));
        EXPECT_TRUE(predictor->_started);

        ctpl::thread_pool p(2);
        vector<std::future<float>> results;
        for (int i = 0; i < 100; ++i) {
            auto f = p.push([&predictor](int id) {
                user_profile user;
                doc_attributes doc;
                context_info cxt;
                recent_doc rdoc;
                rdoc.time = 123456;
                cxt.rdocs.emplace_back(rdoc);
                return predictor->test(user, cxt, doc);
            });
            results.emplace_back(std::move(f));
        }
        p.stop(true);
        p.clear_queue();
        for (int i = 0; i < results.size(); ++i) {
            auto res = results[i].get();
            EXPECT_EQ(res, 123456);
        }
    }

    TEST_F(test_linear_predictor, update_model) {
        string model_path = "test/data/vw_model.txt";
        string model_bak_path = model_path + ".bak";
        string w2v_model_path = "test/data/word2vec_model.txt";
        auto predictor = unique_ptr<linear_predictor>(new linear_predictor(model_path, w2v_model_path, 1));
        EXPECT_TRUE(predictor->_started);
        auto first_time = predictor->_daemon->_last_mod_time.tv_sec;
        if(bfs::exists(model_bak_path)) bfs::remove(model_bak_path);
        bfs::copy_file(model_path, model_bak_path);
        bfs::remove(model_path);
        boost::this_thread::sleep(boost::posix_time::seconds(3));
        auto second_time = predictor->_daemon->_last_mod_time.tv_sec;
        EXPECT_EQ(first_time, second_time);
        bfs::copy_file(model_bak_path, model_path);
        boost::this_thread::sleep(boost::posix_time::seconds(3));
        auto third_time = predictor->_daemon->_last_mod_time.tv_sec;
        EXPECT_GT(third_time, first_time);
    }
} //namespace