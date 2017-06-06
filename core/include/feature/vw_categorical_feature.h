//
// Created by qfeng on 17-6-5.
//

#ifndef PCTR_VW_CATEGORICAL_FEATURE_H
#define PCTR_VW_CATEGORICAL_FEATURE_H

#include "vw_feature.h"
#include <string>
#include <boost/optional.hpp>

using namespace std;
using boost::optional;
using boost::none;

class vw_categorical_feature: public vw_feature {
public:
    string _ns;
    optional<string> _name = none;

    vw_categorical_feature(string ns, optional<string> name, optional<string> name_prefix = none);
    vw_categorical_feature(string ns, string name, optional<int> value, optional<string> format = none);
    vw_categorical_feature(string ns, string name, optional<short> value, optional<string> format = none);
    vw_categorical_feature(string ns, string name, optional<bool> value, bool maybe_missing = false);
    optional<tuple<string, string, float>> dumps();
};

#endif //PCTR_VW_CATEGORICAL_FEATURE_H
