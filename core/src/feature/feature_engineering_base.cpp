//
// Created by qfeng on 17-6-6.
//

#include "feature/feature_engineering_base.h"

void feature_engineering_base::fill_features(features &f,
                                             user_profile &user,
                                             context_info &cxt,
                                             doc_attributes &doc) {
    f._user_id = user.user_id;
    f._gender = user.gender;
    f._doc_id = doc.doc_id;
    f._hotness = doc.hotness;
    f._quality = doc.quality;
    f._position = doc.position;
    f._vulgar = doc.vulgar;
    f._time = cxt.time;
    f._platform = cxt.platform;
    f._brand = cxt.brand;
    f._network = cxt.network;
    f._location0 = cxt.location0;
    f._location1 = cxt.location1;
    f._location2 = cxt.location2;
}