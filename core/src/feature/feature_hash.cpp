//
// Created by qfeng on 17-5-22.
//

#include <cstdint>
#include "feature/feature_hash.h"
#include "vw_hash/constant.h"
#include "vw_hash/hash.h"

feature_hash::feature_hash(uint64_t size) {
    _size = size;
}

uint64_t feature_hash::size() const {
    return _size;
}

bool feature_hash::resize(uint64_t size) {
    _size = size;
}

uint64_t feature_hash::hash_constant() const {
    return bias_constant % (_size);
}

uint64_t feature_hash::hash_namespace(const string& ns) const {
    if (ns.size() > 0) {
        return hash_string(ns, hash_base);
    } else return 0;
}

uint64_t feature_hash::hash_single_feature(const string& ns, const string& feature) const {
    if (feature.size() > 0) {
        uint64_t ns_hash = hash_namespace(ns);
        return hash_string(feature, ns_hash) % (_size);
    } else return 0;
}

uint64_t feature_hash::hash_quadratic_features(const string& lhs_ns,
                                               const string& lhs_feature,
                                               const string& rhs_ns,
                                               const string& rhs_feature) const {
    uint64_t lhs_hash = hash_single_feature(lhs_ns, lhs_feature);
    uint64_t rhs_hash = hash_single_feature(rhs_ns, rhs_feature);
    return ((lhs_hash * FNV_prime) ^ rhs_hash) % (_size);
}

uint64_t feature_hash::hash_string(const string& s, uint64_t h) const {
    auto iter = s.begin();
    //trim leading whitespace but not UTF-8
    for (; iter < s.end() && *iter <= 0x20 && (int) *iter >= 0; iter++);
    iter = s.end();
    //trim trailing white space but not UTF-8
    for (; iter > s.begin() && *(iter - 1) <= 0x20 && (int) *(iter - 1) >= 0; iter--);

    size_t ret = 0;
    iter = s.begin();
    while (iter != s.end())
        if (*iter >= '0' && *iter <= '9')
            ret = 10 * ret + *(iter++) - '0';
        else
            return uniform_hash((unsigned char *) s.c_str(), s.length(), h);

    return ret + h;
}