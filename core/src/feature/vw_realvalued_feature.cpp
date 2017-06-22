//
// Created by qfeng on 17-6-5.
//

#include "feature/vw_realvalued_feature.h"

vw_realvalued_feature::vw_realvalued_feature(const string& ns, const string& name, optional<float> value) {
    _ns = ns;
    _name = name;
    _value = value;
}

vw_realvalued_feature::vw_realvalued_feature(const string& ns,
                                             const string& name,
                                             optional<float> diff_lhs,
                                             optional<float> diff_rhs) {
    if (diff_lhs && diff_rhs) {
        _ns = ns;
        _name = name;
        _value = diff_lhs.get() - diff_rhs.get();
    }
}

optional<tuple<string, string, float>> vw_realvalued_feature::dumps() {
    if (_value)
        return std::make_tuple(_ns, _name, _value.get());
    else
        return boost::none;
}