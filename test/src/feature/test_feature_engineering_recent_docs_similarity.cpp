//
// Created by qfeng on 17-6-7.
//

#include <gtest/gtest.h>
#include <fstream>
#include "feature/feature_engineering_recent_docs_similarity.h"
#include "json/json_wrap.h"

using json = nlohmann::json;

namespace ctr_test {

    class test_feature_engineering_recent_docs_similarity : public ::testing::Test {
    protected:
        std::ifstream _samples;
        unique_ptr<feature_hash> _hash;
        unique_ptr<word2vec_model> _w2v_model;
        unique_ptr<feature_engineering_recent_docs_similarity> _fe_recent_docs_sim;

        virtual void SetUp() {
            _samples.open("test/data/feature_samples.json");
            _hash.reset(new feature_hash(pow(2, 30)));
            _w2v_model = std::move(word2vec_model::load_model("test/data/word2vec.sample.vec"));
            _fe_recent_docs_sim.reset(new feature_engineering_recent_docs_similarity(1 << 30, 0.1, *_w2v_model));
        }

        virtual void TearDown() {
            if (_samples.is_open()) _samples.close();
        }

        bool cache_recent_doc_time(json &j, map<string, unsigned long> &output_doc_time_cache) {
            auto input_impression_recent_docs = json_wrap::get_json(j, "impression", "recentDocs");
            if (input_impression_recent_docs) {
                for (auto &rdoc : json::iterator_wrapper(input_impression_recent_docs.get())) {
                    auto k = json_wrap::get<string>(rdoc.value(), "k");
                    auto v = json_wrap::get<string>(rdoc.value(), "v");
                    if (k && v) {
                        output_doc_time_cache.insert(make_pair(k.get(), (unsigned long) atol(v.get().c_str())));
                    }
                }
                if (output_doc_time_cache.size() > 0) return true;
            } else return false;
        }

        bool extract_recent_docs_from_json(json &j, map<string, unsigned long> &doc_time_cache,
                                           vector<recent_doc> &output_rdocs) {
            auto input_recent_docs = json_wrap::get_json(j, "recentDocs");
            if (input_recent_docs) {
                for (auto &rdoc: json::iterator_wrapper(input_recent_docs.get())) {
                    auto rdoc_id = json_wrap::get<string>(rdoc.value(), "docID");
                    if (rdoc_id && doc_time_cache.find(rdoc_id.get()) != doc_time_cache.end()) {
                        recent_doc output_rdoc;
                        output_rdoc.time = doc_time_cache[rdoc_id.get()];
                        extract_iterable_from_json(rdoc.value(), "category", "", output_rdoc.categories);
                        extract_iterable_from_json(rdoc.value(), "keywords", "", output_rdoc.keywords);
                        output_rdocs.push_back(output_rdoc);
                    }
                }
                if (output_rdocs.size() > 0) return true;
            }
            return false;
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


    TEST_F(test_feature_engineering_recent_docs_similarity, general) {
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

            auto ctx_time = json_wrap::get<unsigned long>(j_flatten, "/impression/time");
            if (ctx_time) cxt.time = ctx_time.get(); else continue;

            map<string, unsigned long> doc_time_cache;
            if (!cache_recent_doc_time(j, doc_time_cache)) continue;
            bool recent_docs_succ = extract_recent_docs_from_json(j, doc_time_cache, cxt.rdocs);
            bool doc_category_succ = extract_iterable_from_json(j, "doc", "category", doc.categories);
            bool doc_keywords_succ = extract_iterable_from_json(j, "doc", "keywords", doc.keywords);

            _fe_recent_docs_sim->fill_features(f, user, cxt, doc);

            if (doc_category_succ && recent_docs_succ) {
                check_feature(j_flatten, "/feature/recent_doc_1min_category_cosine_similarity",
                              f._recent_doc_1min_category_cosine_similarity);

                check_feature(j_flatten, "/feature/recent_doc_5min_category_cosine_similarity",
                              f._recent_doc_5min_category_cosine_similarity);

                check_feature(j_flatten, "/feature/recent_doc_10min_category_cosine_similarity",
                              f._recent_doc_10min_category_cosine_similarity);

                check_feature(j_flatten, "/feature/recent_doc_longtime_category_cosine_similarity",
                              f._recent_doc_longtime_category_cosine_similarity);
            }

            if (doc_keywords_succ && recent_docs_succ) {
                check_feature(j_flatten, "/feature/recent_doc_1min_keyword_cosine_similarity",
                              f._recent_doc_1min_keyword_cosine_similarity);

                check_feature(j_flatten, "/feature/recent_doc_5min_keyword_cosine_similarity",
                              f._recent_doc_5min_keyword_cosine_similarity);

                check_feature(j_flatten, "/feature/recent_doc_10min_keyword_cosine_similarity",
                              f._recent_doc_10min_keyword_cosine_similarity);

                check_feature(j_flatten, "/feature/recent_doc_longtime_keyword_cosine_similarity",
                              f._recent_doc_longtime_keyword_cosine_similarity);

                check_feature(j_flatten, "/feature/recent_doc_1min_keyword_w2v_similarity",
                              f._recent_doc_1min_keyword_w2v_similarity);

                check_feature(j_flatten, "/feature/recent_doc_5min_keyword_w2v_similarity",
                              f._recent_doc_5min_keyword_w2v_similarity);

                check_feature(j_flatten, "/feature/recent_doc_10min_keyword_w2v_similarity",
                              f._recent_doc_10min_keyword_w2v_similarity);

                check_feature(j_flatten, "/feature/recent_doc_longtime_keyword_w2v_similarity",
                              f._recent_doc_longtime_keyword_w2v_similarity);
            }
        }
    }
} //namespace

