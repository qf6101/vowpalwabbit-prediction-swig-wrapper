//
// Created by qfeng on 17-6-7.
//

#include <gtest/gtest.h>
#include <fstream>
#include "feature/feature_engineering_base.h"
#include "json/json_wrap.h"

using json = nlohmann::json;

namespace ctr_test {

    class test_feature_engineering_base : public ::testing::Test {
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

    TEST_F(test_feature_engineering_base, general) {
        string line;
        while (!_samples.eof()) {
            features f(*_hash);
            user_profile user;
            context_info cxt;
            doc_attributes doc;

            getline(_samples, line);
            if (line.length() == 0) continue;
            auto j = json::parse(line).flatten();

            auto input_user_uid = json_wrap::get<string>(j, "/user/uid");
            if (input_user_uid) user.user_id = input_user_uid.get();

            auto input_user_gender = json_wrap::get<string>(j, "/impression/gender");
            if (input_user_gender) user.gender = input_user_gender.get();

            auto input_doc_id = json_wrap::get<string>(j, "/doc/docID");
            if (input_doc_id) doc.doc_id = input_doc_id.get();

            auto input_doc_type = json_wrap::get<string>(j, "/impression/docType");
            if (input_doc_type) doc.doc_type = input_doc_type.get();

            auto input_doc_hotness = json_wrap::get<float>(j, "/doc/hotness");
            if (input_doc_hotness) doc.hotness = input_doc_hotness.get();

            auto input_doc_quality = json_wrap::get<float>(j, "/doc/quality");
            if (input_doc_quality) doc.quality = input_doc_quality.get();

            auto input_doc_vulgar = json_wrap::get<float>(j, "/doc/vulgar");
            if (input_doc_vulgar) doc.vulgar = input_doc_vulgar.get();

            auto input_cxt_platform = json_wrap::get<string>(j, "/impression/platform");
            if (input_cxt_platform) cxt.platform = input_cxt_platform.get();

            auto input_cxt_brand = json_wrap::get<string>(j, "/impression/brand");
            if (input_cxt_brand) cxt.brand = input_cxt_brand.get();

            auto input_cxt_network = json_wrap::get<string>(j, "/impression/network");
            if (input_cxt_network) cxt.network = input_cxt_network.get();

            feature_engineering_base::fill_features(f, user, cxt, doc);

            if (input_user_uid) EXPECT_EQ(f._user_id.get(), input_user_uid.get());
            if (input_user_gender) EXPECT_EQ(f._gender.get(), input_user_gender.get());
            if (input_doc_id) EXPECT_EQ(f._doc_id.get(), input_doc_id.get());
            if (input_doc_type) EXPECT_EQ(f._doc_type.get(), input_doc_type.get());
            if (input_doc_hotness) {
                if(input_doc_hotness.get() >= 0) EXPECT_NEAR(f._hotness.get(), input_doc_hotness.get(), 1E-8);
                else EXPECT_TRUE(!f._hotness);
            }
            if (input_doc_quality) {
                if(input_doc_quality.get() >= 0) EXPECT_NEAR(f._quality.get(), input_doc_quality.get(), 1E-8);
                else EXPECT_TRUE(!f._quality);
            }
            if (input_doc_vulgar) {
                if(input_doc_vulgar.get() >= 0) EXPECT_NEAR(f._vulgar.get(), input_doc_vulgar.get(), 1E-8);
                else EXPECT_TRUE(!f._vulgar);
            }
            if (input_cxt_platform) EXPECT_EQ(f._platform.get(), input_cxt_platform.get());
            if (input_cxt_brand) EXPECT_EQ(f._brand.get(), input_cxt_brand.get());
            if (input_cxt_network) EXPECT_EQ(f._network.get(), input_cxt_network.get());
        }
    }
} //namespace

