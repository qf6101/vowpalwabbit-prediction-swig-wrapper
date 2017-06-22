//
// Created by qfeng on 17-6-6.
//

#ifndef PCTR_FEATURE_ENGINEERING_TIME_H
#define PCTR_FEATURE_ENGINEERING_TIME_H

#include "features.h"
#include "attribute/user_profile.h"
#include "attribute/context_info.h"
#include "attribute/doc_attributes.h"

class feature_engineering_time {
public:
    static void fill_features(features& f, const user_profile& user, const context_info& cxt, const doc_attributes& doc);
    static short get_time_intraday_period(short hour);
};


#endif //PCTR_FEATURE_ENGINEERING_TIME_H
