//
// Created by qfeng on 17-6-6.
//

#include <boost/algorithm/string.hpp>
#include <log/pctr_log.h>
#include "feature/feature_engineering_base.h"
#include "feature/feature_engineering_location.h"

void feature_engineering_location::fill_features(features &f,
                                                 const user_profile &user,
                                                 const context_info &cxt,
                                                 const doc_attributes &doc) {
    f._location0 = feature_engineering_base::trim_string(cxt.location0);
    f._location1 = feature_engineering_base::trim_string(cxt.location1);
    f._location2 = feature_engineering_base::trim_string(cxt.location2);

    if(f._location0 && f._location1) f._location1 = f._location0.get() + "/" + f._location1.get();
    if(f._location1 && f._location2) f._location2 = f._location1.get() + "/" + f._location2.get();

    f._doc_location0 = feature_engineering_base::trim_string(doc.position0);
    f._doc_location1 = feature_engineering_base::trim_string(doc.position1);
    f._doc_location2 = feature_engineering_base::trim_string(doc.position2);

    if(f._doc_location0 && f._doc_location1) f._doc_location1 = f._doc_location0.get() + "/" + f._doc_location1.get();
    if(f._doc_location1 && f._doc_location2) f._doc_location2 = f._doc_location1.get() + "/" + f._doc_location2.get();

    if (f._location0 && f._doc_location0) f._same_location0 = f._location0.get() == f._doc_location0.get();
    if (f._location1 && f._doc_location1) f._same_location1 = f._location1.get() == f._doc_location1.get();
    if (f._location2 && f._doc_location2) f._same_location2 = f._location2.get() == f._doc_location2.get();
}

vector<string> feature_engineering_location::parse_doc_location(const string& doc_position) {
    auto logger = spdlog::get(pctr_log::_core_logger_name);
    try {
        vector<string> splits;
        boost::algorithm::split(splits, doc_position, boost::algorithm::is_any_of(","));
        if (splits.size() != 4 || splits[3] == "false")
            return vector<string>{};
        else {
            vector<string> res;
            if (splits[0] != "null") {
                res.push_back(splits[0]);
                if (splits[1] != "null") {
                    res.push_back(splits[0] + "/" + splits[1]);
                    if (splits[2] != "null")
                        res.push_back(splits[0] + "/" + splits[1] + "/" + splits[2]);
                }
            }
            return res;
        }
    } catch (exception &ex) {
        logger->debug("Fail to parse document's position={}", doc_position);
        return vector<string>{};
    }

}