//
// Created by qfeng on 17-6-2.
//

#include "feature/feature_hash.h"
#include "feature/features.h"
#include "feature/vw_categorical_feature.h"
#include "feature/vw_realvalued_feature.h"

unique_ptr<Eigen::SparseVector<float>> features::to_vw() {
    try {
        vector<optional<tuple<string, string, float>>> before_hashing{
                vw_categorical_feature("gender", _gender).dumps(),
                vw_categorical_feature("platform", _platform).dumps(),
                vw_categorical_feature("brand", _brand).dumps(),
                vw_categorical_feature("network", _network).dumps(),
                vw_categorical_feature("location", _location0, string("0")).dumps(),
                vw_categorical_feature("location", _location1, string("1")).dumps(),
                vw_categorical_feature("doc", _doc_type, string("type")).dumps(),
                vw_realvalued_feature("doc", "hotness", _hotness).dumps(),
                vw_realvalued_feature("doc", "quality", _quality).dumps(),
                vw_realvalued_feature("doc", "vulgar", _vulgar).dumps(),
                vw_categorical_feature("doc", "publish_hour", _publish_hour, string("%02d")).dumps(),
                vw_categorical_feature("doc", "publish_hour_desc", _publish_hour_desc).dumps(),
                vw_categorical_feature("doc", _doc_location0, string("location0")).dumps(),
                vw_categorical_feature("doc", _doc_location1, string("location1")).dumps(),
                vw_categorical_feature("view_time", "day_of_week", _weekday).dumps(),
                vw_categorical_feature("view_time", "is_workday", _is_workday).dumps(),
                vw_categorical_feature("view_time", "hour", _hour, string("%02d")).dumps(),
                vw_categorical_feature("view_time", "hour_desc", _hour_desc).dumps(),
                vw_realvalued_feature("view_doc_delay", "days", _view_and_doc_delay_days).dumps(),
                vw_categorical_feature("view_doc_delay", "in_same_day", _view_and_doc_in_same_day).dumps(),
                vw_categorical_feature("view_doc_delay", "desc", _view_and_doc_delay_desc).dumps(),
                vw_realvalued_feature("user_doc_similarity", "w2v", _user_doc_keyword_w2v_similarity).dumps(),
                vw_realvalued_feature("user_doc_similarity", "cosine", _user_doc_keyword_cosine_similarity).dumps(),
                vw_realvalued_feature("user_doc_similarity", "category_jaccard",
                                      _user_doc_category_jaccard_similarity).dumps(),
                vw_realvalued_feature("user_doc_similarity", "category_cosine",
                                      _user_doc_category_cosine_similarity).dumps(),
                vw_categorical_feature("same_location", "0", _same_location0).dumps(),
                vw_categorical_feature("same_location", "1", _same_location1).dumps(),
                vw_categorical_feature("same_location", "2", _same_location2).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "w2v_1min",
                                      _recent_doc_1min_keyword_w2v_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "w2v_5min",
                                      _recent_doc_5min_keyword_w2v_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "w2v_10min",
                                      _recent_doc_10min_keyword_w2v_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "w2v_long",
                                      _recent_doc_longtime_keyword_w2v_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "cosine_1min",
                                      _recent_doc_1min_keyword_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "cosine_5min",
                                      _recent_doc_5min_keyword_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "cosine_10min",
                                      _recent_doc_10min_keyword_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "cosine_long",
                                      _recent_doc_longtime_keyword_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "category_cosine_1min",
                                      _recent_doc_1min_category_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "category_cosine_5min",
                                      _recent_doc_5min_category_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "category_cosine_10min",
                                      _recent_doc_10min_category_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_similarity", "category_cosine_long",
                                      _recent_doc_longtime_category_cosine_similarity).dumps(),
                vw_realvalued_feature("recent_docs_diff", "hotness_1min",
                                      _hotness, _recent_doc_1min_avg_hotness).dumps(),
                vw_realvalued_feature("recent_docs_diff", "hotness_5min",
                                      _hotness, _recent_doc_5min_avg_hotness).dumps(),
                vw_realvalued_feature("recent_docs_diff", "hotness_10min",
                                      _hotness, _recent_doc_10min_avg_hotness).dumps(),
                vw_realvalued_feature("recent_docs_diff", "hotness_long",
                                      _hotness, _recent_doc_longtime_avg_hotness).dumps(),
                vw_realvalued_feature("recent_docs_diff", "quality_1min",
                                      _quality, _recent_doc_1min_avg_quality).dumps(),
                vw_realvalued_feature("recent_docs_diff", "quality_5min",
                                      _quality, _recent_doc_5min_avg_quality).dumps(),
                vw_realvalued_feature("recent_docs_diff", "quality_10min",
                                      _quality, _recent_doc_10min_avg_quality).dumps(),
                vw_realvalued_feature("recent_docs_diff", "quality_long",
                                      _quality, _recent_doc_longtime_avg_quality).dumps(),
                vw_realvalued_feature("recent_docs_diff", "vulgar_1min",
                                      _vulgar, _recent_doc_1min_avg_vulgar).dumps(),
                vw_realvalued_feature("recent_docs_diff", "vulgar_5min",
                                      _vulgar, _recent_doc_5min_avg_vulgar).dumps(),
                vw_realvalued_feature("recent_docs_diff", "vulgar_10min",
                                      _vulgar, _recent_doc_10min_avg_vulgar).dumps(),
                vw_realvalued_feature("recent_docs_diff", "vulgar_long",
                                      _vulgar, _recent_doc_longtime_avg_vulgar).dumps()
        };

        auto vw_features = unique_ptr<Eigen::SparseVector<float>>(new Eigen::SparseVector<float>(_hash.size()));

        for_each(before_hashing.begin(), before_hashing.end(),
                 [this, &vw_features](optional<tuple<string, string, float>> feature) {
                     if (feature.is_initialized()) {
                         tuple<string, string, float> &f = feature.get();
                         auto &ns = std::get<0>(f);
                         auto &name = std::get<1>(f);
                         auto &value = std::get<2>(f);
                         if (isfinite(value)) {
                             auto hash_code = _hash.hash_single_feature(ns, name);
                             vw_features->coeffRef(hash_code) = value;
                         }
                     }
                 });

        return vw_features;
    } catch (exception &ex) {
        return nullptr;
    }
}