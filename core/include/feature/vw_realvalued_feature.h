//
// Created by qfeng on 17-6-5.
//

#ifndef PCTR_VW_REALVALUED_FEATURE_H
#define PCTR_VW_REALVALUED_FEATURE_H

#include <string>
#include <boost/optional.hpp>
#include "vw_feature.h"

using namespace std;
using boost::optional;
using boost::none;

class vw_realvalued_feature: public vw_feature {
public:
    string _ns;
    string _name;
    optional<float> _value = none;

    vw_realvalued_feature(const string& ns, const string& name, optional<float> value);
    vw_realvalued_feature(const string& ns, const string& name, optional<float> diff_lhs, optional<float> diff_rhs);
    optional<tuple<string, string, float>> dumps();
};

#endif //PCTR_VW_REALVALUED_FEATURE_H
