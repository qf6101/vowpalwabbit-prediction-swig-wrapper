//
// Created by qfeng on 17-6-2.
//

#ifndef PCTR_FEATURES_H
#define PCTR_FEATURES_H

#include <Eigen/Sparse>
#include <string>
#include <boost/optional.hpp>

using namespace std;
using boost::optional;
using boost::none;

class features {
public:
    optional<string> _user_id = none;
    optional<string> _doc_id = none;
    optional<long> _time = none;
    optional<string> _gender = none;
    optional<string> _platform = none;
    optional<string> _brand = none;
    optional<string> _network = none;
    optional<string> _location0 = none;
    optional<string> _location1 = none;
    optional<string> _location2 = none;
    optional<short> _weekday = none;
    optional<bool> _is_workday = none;
    optional<short> _hour = none;
    optional<short> _hour_desc = none;
    optional<string> _doc_type = none;
    optional<float> _hotness = none;
    optional<string> _position = none;
    optional<float> _quality = none;
    optional<float> _vulgar = none;
    optional<string> _doc_location0 = none;
    optional<string> _doc_location1 = none;
    optional<string> _doc_location2 = none;
    optional<short> _publish_hour = none;
    optional<short> _publish_hour_desc = none;
    optional<float> _view_and_doc_delay_days = none;
    optional<bool> _view_and_doc_in_same_day = none;
    optional<short> _view_and_doc_delay_desc = none;
    optional<bool> _same_location0 = none;
    optional<bool> _same_location1 = none;
    optional<bool> _same_location2 = none;
    optional<float> _user_doc_keyword_w2v_similarity = none;
    optional<float> _user_doc_keyword_cosine_similarity = none;
    optional<float> _user_doc_category_jaccard_similarity = none;
    optional<float> _user_doc_category_cosine_similarity = none;
    optional<float> _recent_doc_1min_keyword_w2v_similarity = none;
    optional<float> _recent_doc_5min_keyword_w2v_similarity = none;
    optional<float> _recent_doc_10min_keyword_w2v_similarity = none;
    optional<float> _recent_doc_longtime_keyword_w2v_similarity = none;
    optional<float> _recent_doc_1min_keyword_cosine_similarity = none;
    optional<float> _recent_doc_5min_keyword_cosine_similarity = none;
    optional<float> _recent_doc_10min_keyword_cosine_similarity = none;
    optional<float> _recent_doc_longtime_keyword_cosine_similarity = none;
    optional<float> _recent_doc_1min_category_cosine_similarity = none;
    optional<float> _recent_doc_5min_category_cosine_similarity = none;
    optional<float> _recent_doc_10min_category_cosine_similarity = none;
    optional<float> _recent_doc_longtime_category_cosine_similarity = none;
    optional<float> _recent_doc_1min_avg_hotness = none;
    optional<float> _recent_doc_5min_avg_hotness = none;
    optional<float> _recent_doc_10min_avg_hotness = none;
    optional<float> _recent_doc_longtime_avg_hotness = none;
    optional<float> _recent_doc_1min_avg_quality = none;
    optional<float> _recent_doc_5min_avg_quality = none;
    optional<float> _recent_doc_10min_avg_quality = none;
    optional<float> _recent_doc_longtime_avg_quality = none;
    optional<float> _recent_doc_1min_avg_vulgar = none;
    optional<float> _recent_doc_5min_avg_vulgar = none;
    optional<float> _recent_doc_10min_avg_vulgar = none;
    optional<float> _recent_doc_longtime_avg_vulgar = none;

    void to_vw(Eigen::SparseVector<float>** vw_features);
};


#endif //PCTR_FEATURES_H
