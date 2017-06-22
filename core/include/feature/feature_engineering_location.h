//
// Created by qfeng on 17-6-6.
//

#ifndef PCTR_FEATURE_ENGINEERING_LOCATION_H
#define PCTR_FEATURE_ENGINEERING_LOCATION_H

#include "attribute/user_profile.h"
#include "attribute/context_info.h"
#include "attribute/doc_attributes.h"
#include "feature/features.h"


class feature_engineering_location {
public:
    static void fill_features(features& f, const user_profile& user, const context_info& cxt, const doc_attributes& doc);
    static vector<string> parse_doc_location(const string& doc_position);
};


#endif //PCTR_FEATURE_ENGINEERING_LOCATION_H
