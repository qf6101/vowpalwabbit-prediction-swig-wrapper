//
// Created by qfeng on 17-6-6.
//

#ifndef PCTR_FEATURE_ENGINEERING_BASE_H
#define PCTR_FEATURE_ENGINEERING_BASE_H

#include "attribute/user_profile.h"
#include "attribute/context_info.h"
#include "attribute/doc_attributes.h"
#include "features.h"

class feature_engineering_base {
public:
    static void fill_features(features& f, const user_profile& user, const context_info& cxt, const doc_attributes& doc);
    static optional<string> trim_string(const string& input);
};


#endif //PCTR_FEATURE_ENGINEERING_BASE_H
