//
// Created by qfeng on 17-6-7.
//

#include <gtest/gtest.h>
#include <fstream>
#include "feature/feature_engineering_time.h"
#include "json/json_wrap.h"

using json = nlohmann::json;

namespace ctr_test {

    class test_feature_engineering_time : public ::testing::Test {
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


    TEST_F(test_feature_engineering_time, general) {
        string line;
        while (!_samples.eof()) {
            features f(*_hash);
            user_profile user;
            context_info cxt;
            doc_attributes doc;

            getline(_samples, line);
            if (line.length() == 0) continue;
            auto j = json::parse(line).flatten();

            auto input_cxt_time = json_wrap::get<unsigned long>(j, "/impression/time");
            if (input_cxt_time) cxt.time = input_cxt_time.get();

            auto input_doc_ptime = json_wrap::get<unsigned long>(j, "/doc/ptime");
            if (input_doc_ptime) doc.ptime = input_doc_ptime.get();

            feature_engineering_time::fill_features(f, user, cxt, doc);

            if (input_cxt_time) {
                auto output_time = json_wrap::get<unsigned long>(j, "/feature/time");
                auto output_weekday = json_wrap::get<short>(j, "/feature/weekday");
                auto output_is_work_day = json_wrap::get<bool>(j, "/feature/is_workday");
                auto output_hour = json_wrap::get<short>(j, "/feature/hour");
                auto output_hour_desc = json_wrap::get<short>(j, "/feature/hour_desc");

                EXPECT_EQ(f._time.get(), output_time.get());
                EXPECT_EQ(f._weekday.get(), output_weekday.get());
                EXPECT_EQ(f._is_workday.get(), output_is_work_day.get());
                EXPECT_EQ(f._hour.get(), output_hour.get());
                EXPECT_EQ(f._hour_desc.get(), output_hour_desc.get());
            }

            if (input_doc_ptime) {
                auto output_publish_hour = json_wrap::get<short>(j, "/feature/publish_hour");
                auto output_publish_hour_desc = json_wrap::get<short>(j, "/feature/publish_hour_desc");

                EXPECT_EQ(f._publish_hour.get(), output_publish_hour.get());
                EXPECT_EQ(f._publish_hour_desc.get(), output_publish_hour_desc.get());
            }

            if(input_cxt_time && input_doc_ptime) {
                auto output_view_and_doc_in_same_day = json_wrap::get<bool>(j, "/feature/view_and_doc_in_same_day");
                auto output_view_and_doc_delay_days = json_wrap::get<float>(j, "/feature/view_and_doc_delay_days");
                auto output_view_and_doc_delay_desc = json_wrap::get<short>(j, "/feature/view_and_doc_delay_desc");

                EXPECT_EQ(f._view_and_doc_in_same_day.get(), output_view_and_doc_in_same_day.get());
                EXPECT_NEAR(f._view_and_doc_delay_days.get(), output_view_and_doc_delay_days.get(), 1E-8);
                EXPECT_EQ(f._view_and_doc_delay_desc.get(), output_view_and_doc_delay_desc.get());
            }
        }
    }
} //namespace

