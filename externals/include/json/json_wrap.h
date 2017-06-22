//
// Created by qfeng on 17-6-13.
//

#ifndef PCTR_JSON_WRAP_H
#define PCTR_JSON_WRAP_H

#include "json.hpp"
#include <string>

using std::string;


class json_wrap {
public:
    template<typename T>
    static optional<T> get(nlohmann::json j, string k) {
        if (j.find(k) != j.end()) {
            return j.at(k).get<T>();
        } else return boost::none;
    }

    static optional<nlohmann::json> get_json(nlohmann::json j, string k1, string k2 = "") {
        if (j.find(k1) != j.end()) {
            auto j1 = j.at(k1);
            if (k2 == "") return j1;
            else if (j1.find(k2) != j1.end()) return j1.at(k2);
        } else return boost::none;
    }
};


#endif //PCTR_JSON_WRAP_H
