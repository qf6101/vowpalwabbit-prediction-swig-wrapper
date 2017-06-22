//
// Created by qfeng on 17-6-7.
//

#include <gtest/gtest.h>
#include <fstream>
#include "feature/feature_engineering_user_doc_similarity.h"
#include "json/json_wrap.h"

using json = nlohmann::json;

namespace ctr_test {

    class test_feature_engineering_user_doc_similarity : public ::testing::Test {
    protected:
        std::ifstream _samples;
        unique_ptr<feature_hash> _hash;
        unique_ptr<word2vec_model> _w2v_model;
        unique_ptr<feature_engineering_user_doc_similarity> _fe_user_doc_sim;

        virtual void SetUp() {
            _samples.open("test/data/feature_samples.json");
            _hash.reset(new feature_hash(pow(2, 30)));
            _w2v_model = std::move(word2vec_model::load_model("test/data/word2vec.sample.vec"));
            _fe_user_doc_sim.reset(new feature_engineering_user_doc_similarity(1 << 30, 0.1, *_w2v_model));
        }

        virtual void TearDown() {
            if (_samples.is_open()) _samples.close();
        }

        bool extract_iterable_from_json(json &j, string k1, string k2, vector<pair<string, float>> &output_iterable) {
            auto input_doc_category = json_wrap::get_json(j, k1, k2);
            if (input_doc_category) {
                for (auto &category : json::iterator_wrapper(input_doc_category.get())) {
                    auto name = json_wrap::get<string>(category.value(), "_1");
                    auto value = json_wrap::get<float>(category.value(), "_2");
                    pair<string, float> item(name.get(), value.get());
                    output_iterable.push_back(item);
                }
                if (output_iterable.size() > 0) return true;
            }
            return false;
        }

        void check_feature(json j, string feature_name, boost::optional<float> &real_feature) {
            auto expect_feature = json_wrap::get_json(j, feature_name);
            if (expect_feature) {
                if (expect_feature.get().is_number())
                    EXPECT_NEAR(real_feature.get(), expect_feature.get(), 1E-6);
                else if (expect_feature.get().is_string() && expect_feature.get() == "NaN")
                    EXPECT_TRUE(real_feature && isnan(real_feature.get()));
            }
        }
    };


    TEST_F(test_feature_engineering_user_doc_similarity, general) {
        string line;
        while (!_samples.eof()) {
            features f(*_hash);
            user_profile user;
            context_info cxt;
            doc_attributes doc;

            getline(_samples, line);
            if (line.length() == 0) continue;
            auto j = json::parse(line);
            auto j_flatten = j.flatten();

            bool doc_category_succ = extract_iterable_from_json(j, "doc", "category", doc.categories);
            bool doc_keywords_succ = extract_iterable_from_json(j, "doc", "keywords", doc.keywords);
            bool user_category_succ = extract_iterable_from_json(j, "user", "category", user.category);
            bool user_tagwords_succ = extract_iterable_from_json(j, "user", "tagwords", user.tagwords);

            _fe_user_doc_sim->fill_features(f, user, cxt, doc);

            if (doc_category_succ && user_category_succ) {
                check_feature(j_flatten, "/feature/user_doc_category_cosine_similarity",
                              f._user_doc_category_cosine_similarity);

                check_feature(j_flatten, "/feature/user_doc_category_jaccard_similarity",
                              f._user_doc_category_jaccard_similarity);
            }

            if (doc_keywords_succ && user_tagwords_succ) {
                check_feature(j_flatten, "/feature/user_doc_keyword_cosine_similarity",
                              f._user_doc_keyword_cosine_similarity);

                check_feature(j_flatten, "/feature/user_doc_keyword_w2v_similarity",
                              f._user_doc_keyword_w2v_similarity);
            }
        }
    }
} //namespace

