//
// Created by qfeng on 17-6-5.
//

#include "feature/vw_categorical_feature.h"
#include <sstream>
#include <iomanip>

vw_categorical_feature::vw_categorical_feature(string ns, optional<string> name, optional<string> name_prefix) {
    if (name) {
        _ns = ns;
        if (name_prefix) _name = name_prefix.get() + "__" + name.get();
        else _name = name;
    }
}

vw_categorical_feature::vw_categorical_feature(string ns, string name, optional<int> value, optional<string> format) {
    if (value) {
        _ns = ns;
        if (format) {
            char buffer[50];
            int offset = sprintf(buffer, format.get().c_str(), value.get());
            buffer[offset - 1] = '\n';
            _name = name + "__" + string(buffer);
        } else _name = std::to_string(value.get());
    }
}

vw_categorical_feature::vw_categorical_feature(string ns, string name, optional<short> value, optional<string> format) {
    if (value) {
        _ns = ns;
        if (format) {
            char buffer[50];
            int offset = sprintf(buffer, format.get().c_str(), value.get());
            buffer[offset - 1] = '\n';
            _name = name + "__" + string(buffer);
        } else _name = std::to_string(value.get());
    }
}

vw_categorical_feature::vw_categorical_feature(string ns, string name, optional<bool> value, bool maybe_missing) {
    if (value) {
        _ns = ns;
        if (maybe_missing) {
            if (value.get()) _name = name + "__1"; else _name = name + "__0";
        } else if (value.get()) _name = name;
    }
}

optional<tuple<string, string, float>> vw_categorical_feature::dumps() {
    if (_name)
        return std::make_tuple(_ns, _name.get(), 1.0F);
    else
        return boost::none;
}
