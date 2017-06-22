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

    vw_categorical_feature(const string& ns, const optional<string>& name, const optional<string>& name_prefix = none);
    vw_categorical_feature(const string& ns, const string& name, const optional<int> value, const optional<string>& format = none);
    vw_categorical_feature(const string& ns, const string& name, const optional<short> value, const optional<string>& format = none);
    vw_categorical_feature(const string& ns, const string& name, const optional<bool> value, bool maybe_missing = false);
    optional<tuple<string, string, float>> dumps();
};

#endif //PCTR_VW_CATEGORICAL_FEATURE_H
