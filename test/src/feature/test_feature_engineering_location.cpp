//
// Created by qfeng on 17-6-7.
//

#include <gtest/gtest.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "feature/feature_engineering_location.h"
#include "json/json_wrap.h"

using json = nlohmann::json;
using namespace boost::algorithm;

namespace ctr_test {

    class test_feature_engineering_location : public ::testing::Test {
    protected:
        std::ifstream _samples;
        unique_ptr<feature_hash> _hash;

        virtual void SetUp() {
            _samples.open("test/data/feature_samples.json");
            _hash.reset(new feature_hash(pow(2, 30)));
        }

        virtual void TearDown() {
            if (_samples.is_open()) _samples.close();
        }
    };


    TEST_F(test_feature_engineering_location, general) {
        string line;
        while (!_samples.eof()) {
            features f(*_hash);
            user_profile user;
            context_info cxt;
            doc_attributes doc;

            getline(_samples, line);
            if (line.length() == 0) continue;
            auto j = json::parse(line).flatten();

            auto input_cxt_location0 = json_wrap::get<string>(j, "/impression/location0");
            if (input_cxt_location0) cxt.location0 = input_cxt_location0.get();
            auto input_cxt_location1 = json_wrap::get<string>(j, "/impression/location1");
            if (input_cxt_location1) cxt.location1 = input_cxt_location1.get();
            auto input_cxt_location2 = json_wrap::get<string>(j, "/impression/location2");
            if (input_cxt_location2) cxt.location2 = input_cxt_location2.get();

            optional<string> input_doc_location0 = boost::none;
            optional<string> input_doc_location1 = boost::none;
            optional<string> input_doc_location2 = boost::none;
            auto input_doc_position = json_wrap::get<string>(j, "/doc/position");
            if (input_doc_position && trim_copy(input_doc_position.get()).size() > 0) {
                vector<string> splits;
                split(splits, input_doc_position.get(), is_any_of(","));
                if (splits.size() == 4 || splits[3] == "true") {
                    if (trim_copy(splits[0]).size() > 0) input_doc_location0 = splits[0];
                    if (trim_copy(splits[1]).size() > 0) input_doc_location1 = splits[1];
                    if (trim_copy(splits[2]).size() > 0) input_doc_location2 = splits[2];
                }
            }

            if (input_doc_location0) doc.position0 = input_doc_location0.get();
            if (input_doc_location1) doc.position1 = input_doc_location1.get();
            if (input_doc_location2) doc.position2 = input_doc_location2.get();

            feature_engineering_location::fill_features(f, user, cxt, doc);

            if (input_cxt_location0) EXPECT_EQ(f._location0.get(), input_cxt_location0.get());
            if (input_cxt_location1)
                EXPECT_EQ(f._location1.get(), input_cxt_location0.get() + "/" +
                                              input_cxt_location1.get());
            if (input_cxt_location2)
                EXPECT_EQ(f._location2.get(), input_cxt_location0.get() + "/" +
                                              input_cxt_location1.get() + "/" + input_cxt_location2.get());

            if (input_doc_location0) EXPECT_EQ(f._doc_location0.get(), input_doc_location0.get());
            if (input_doc_location1)
                EXPECT_EQ(f._doc_location1.get(), input_doc_location0.get() + "/" +
                                                  input_doc_location1.get());
            if (input_doc_location2)
                EXPECT_EQ(f._doc_location2.get(), input_doc_location0.get() + "/" +
                                                  input_doc_location1.get() + "/" + input_doc_location2.get());

            if (input_cxt_location0 && input_doc_location0) {
                auto output_same_location0 = json_wrap::get<bool>(j, "/feature/same_location0");
                if (output_same_location0) EXPECT_EQ(f._same_location0.get(), output_same_location0.get());
            }

            if (input_cxt_location1 && input_doc_location1) {
                auto output_same_location1 = json_wrap::get<bool>(j, "/feature/same_location1");
                if (output_same_location1) EXPECT_EQ(f._same_location1.get(), output_same_location1.get());
            }

            if (input_cxt_location2 && input_doc_location2) {
                auto output_same_location2 = json_wrap::get<bool>(j, "/feature/same_location2");
                if (output_same_location2) EXPECT_EQ(f._same_location2.get(), output_same_location2.get());
            }
        }
    }
} //namespace