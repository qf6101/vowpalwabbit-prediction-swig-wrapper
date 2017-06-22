//
// Created by qfeng on 17-6-6.
//

#include "feature/feature_engineering_base.h"
#include <boost/algorithm/string.hpp>

using boost::none;
using namespace boost::algorithm;

void feature_engineering_base::fill_features(features &f,
                                             const user_profile &user,
                                             const context_info &cxt,
                                             const doc_attributes &doc) {
    f._user_id = trim_string(user.user_id);
    f._gender = trim_string(user.gender);
    f._doc_id = trim_string(doc.doc_id);
    f._doc_type = trim_string(doc.doc_type);
    if (doc.hotness >= 0) f._hotness = doc.hotness;
    if (doc.quality >= 0) f._quality = doc.quality;
    if (doc.vulgar >= 0) f._vulgar = doc.vulgar;
    f._platform = trim_string(cxt.platform);
    f._brand = trim_string(cxt.brand);
    f._network = trim_string(cxt.network);
}

optional<string> feature_engineering_base::trim_string(const string &input) {
    string output = trim_copy(input);
    if (output.size() > 0) return output;
    else return none;
}